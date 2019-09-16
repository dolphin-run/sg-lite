#include "scenefantacy.h"

#include "node/sgrectanglenode.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

struct Square
{
    glm::vec2 p[4];
    //int idx;//0-3
    float size;
    float rotation;
    glm::vec2 center;

    int generation = 0;
};

struct SquareNode : SGRectangleNode
{
    int generation;
};

static const int g_find_depth = 8;
static Square g_root;
static void initRoot()
{
    g_root.generation = 0;
    g_root.size = 200;
    g_root.rotation = 0.f;
    g_root.p[0] = glm::vec2(960 - g_root.size / 2, 980);
    g_root.p[1] = glm::vec2(960 + g_root.size / 2, 980);
    g_root.p[2] = glm::vec2(960 + g_root.size / 2, 980 - g_root.size);
    g_root.p[3] = glm::vec2(960 - g_root.size / 2, 980 - g_root.size);
    g_root.center = (g_root.p[0] + g_root.p[2]) / 2.f;
}

/*
p3      p2
1+--==--+0
|      |
|      |
+------+
p0      p1
*/

static std::pair<Square, Square> spawn(const Square &sq, float degree)
{
    const glm::vec2 &p0 = sq.p[2];
    const glm::vec2 &p1 = sq.p[3];
    //const glm::vec2 &p2 = sq.p[0];
    //const glm::vec2 &p3 = sq.p[1];

    glm::vec2 n1t = glm::mix(p0, p1, glm::cos(degree));
    n1t -= p0;
    glm::vec3 n1v3 = glm::rotate(glm::vec3(n1t.x, n1t.y, 0), degree, glm::vec3(0, 0, 1));
    glm::vec2 n1(n1v3);
    n1 += p0;

    glm::vec2 dir1 = glm::normalize(n1 - p0);
    glm::vec2 dir2 = glm::normalize(n1 - p1);

    //right
    Square nsq0;
    nsq0.generation = sq.generation + 1;
    nsq0.size = sq.size * glm::cos(degree);
    nsq0.rotation = sq.rotation - degree;
    nsq0.p[0] = n1;
    nsq0.p[1] = p0;
    nsq0.p[2] = p0 + dir2*nsq0.size;
    nsq0.p[3] = n1 + dir2*nsq0.size;
    nsq0.center = (nsq0.p[0] + nsq0.p[2]) / 2.f;

    //left
    Square nsq1;
    nsq1.generation = sq.generation + 1;
    nsq1.size = sq.size * glm::sin(degree);
    nsq1.rotation = sq.rotation + glm::pi<float>() / 2 - degree;
    nsq1.p[0] = p1;
    nsq1.p[1] = n1;
    nsq1.p[2] = n1 + dir1*nsq1.size;
    nsq1.p[3] = p1 + dir1*nsq1.size;
    nsq1.center = (nsq1.p[0] + nsq1.p[2]) / 2.f;

    return std::make_pair(nsq0, nsq1);
}

static void spawnTree(const Square &root, float degree, int depth, std::vector<Square> &result)
{
    result.push_back(root);

    if (depth > 0)
    {
        auto children = spawn(root, glm::radians(degree));
        spawnTree(children.first, degree, depth - 1, result);
        spawnTree(children.second, degree, depth - 1, result);
    }
}

//scene implementation
std::vector<Square> result;
std::mutex mtx;
std::condition_variable cv;

SceneFantacy::SceneFantacy()
{
    initRoot();
    testAlloc();

    m_timer =  new Later(20, true, -1, [this] {this->timerEvent(); });
}

SceneFantacy::~SceneFantacy()
{
    m_timer->stop();
    delete m_timer;
}

void SceneFantacy::timerEvent()
{
    static int igen = 0;
    static bool workerStart = false;
    {
        if (igen <= g_find_depth)
        {
            testVisible(igen++);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            if (workerStart)
            {
                testUpdate();
            }
            else
            {
                std::thread worker(std::thread(&SceneFantacy::testSpawn, this));
                worker.detach();
                workerStart = true;
            }
        }
    }
}

void SceneFantacy::testAlloc()
{
    std::vector<Square> result;
    spawnTree(g_root, 45, g_find_depth, result);

    for (auto sq: result)
    {
        SquareNode *sn = new SquareNode();
        sn->setX(sq.center.x - sq.size / 2);
        sn->setY(sq.center.y - sq.size / 2);
        sn->setRotation(sq.rotation);
        sn->setWidth(sq.size);
        sn->setHeight(sq.size);

        sn->setColor(Color::random());
        sn->setVisible(false);
        sn->generation = sq.generation;
        this->addItem(sn);

        m_nodes.push_back(sn);
    }
}

void SceneFantacy::testVisible(int gen)
{
    for (auto node : m_nodes)
    {
        auto sn = dynamic_cast<SquareNode*>(node);
        if (sn->generation < gen)
            sn->setVisible();
    }
}

void SceneFantacy::testSpawn()
{
    static int dval = 150;
    static bool dir = true;
    std::unique_lock<std::mutex> lk(mtx);

    while (true)
    {
        cv.wait(lk);
        result.clear();
        spawnTree(g_root, 30 + dval / 10.f, g_find_depth, result);

        dir ? dval += 2 : dval -= 2;
        if (dval <= 0 || dval >= 300)
        {
            dir = !dir;
        }
    }
}

void SceneFantacy::testUpdate()
{
    {
        std::lock_guard<std::mutex> lk(mtx);

        for (int i = 0; i < result.size(); i++)
        {
            const Square &sq = result.at(i);
            auto sn = m_nodes.at(i);
            sn->setPos(sq.center.x - sq.size / 2, sq.center.y - sq.size / 2);
            sn->setSize(sq.size, sq.size);
            sn->setRotation(sq.rotation);
        }
    }

    cv.notify_one();
}
