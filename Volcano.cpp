#include "SFML/Graphics.hpp"
#include <vector>

using namespace std;

float timestep = 0.1f;
float acceleration = 9.81f;
int winWid = 800, winHig = 800;
int particleCount = 800;
const int typeCount = 4;
const float initVel = 150.0f;
const int pEmitY = 3.2 * winHig / 4;

class Particle {
public:
    Particle(sf::Vector2f p, sf::Vector2f iv, sf::Color c) {
        pos = p;
        vel = iv;
        color = c;
        checkVisibility();
    }

    sf::Color getColor() { return color; }
    sf::Vector2f getVel() { return vel; }
    sf::Vector2f getPos() { return pos; }

    void updatePos() {
        pos.x += vel.x * timestep;
        pos.y += vel.y * timestep;
    }

    void updateVel() {
        vel.y += acceleration * timestep;
    }

    bool isVisible() {
        checkVisibility();
        return visible;
    }

    void setPos(sf::Vector2f p) {
        pos = p;
    }
    void setVel(sf::Vector2f v) {
        vel = v;
    }
    void setColor(sf::Color c) {
        color = c;
    }

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;
    bool visible;

    void checkVisibility() {
        visible = (pos.x >= 0 && pos.x < winWid&& pos.y >= 0 && pos.x < winHig);
    }
};

float randFloat(float LO, float HI) {
    return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(winWid, winHig), "Volcanic Eruption");
    window.setFramerateLimit(60);
    sf::CircleShape cir(3);
    sf::RectangleShape legendBg(sf::Vector2f(75, 125));
    legendBg.setFillColor(sf::Color::Black);
    legendBg.setPosition(sf::Vector2f(25, 25));
    sf::CircleShape tri(300, 3);
    
    vector<Particle> particles;

    //SO2, CO2, H2S, H20
    float masses[] = {1, 1.45f, 1.9f, 3.6f};
    sf::Color colors[] = { {255, 255, 0}, {255, 198, 100}, {255, 0, 0}, {128, 128, 255} };

    sf::Font font;
    font.loadFromFile("Minecraft.ttf");

    sf::Text so2;
    sf::Text co2;
    sf::Text hs2;
    sf::Text h20;
    so2.setFillColor(colors[0]);
    co2.setFillColor(colors[1]);
    hs2.setFillColor(colors[2]);
    h20.setFillColor(colors[3]);
    so2.setCharacterSize(25);
    co2.setCharacterSize(25);
    hs2.setCharacterSize(25);
    h20.setCharacterSize(25);
    so2.setPosition(sf::Vector2f(30, 30));
    co2.setPosition(sf::Vector2f(30, 60));
    hs2.setPosition(sf::Vector2f(30, 90));
    h20.setPosition(sf::Vector2f(30, 120));
    so2.setFont(font);
    co2.setFont(font);
    hs2.setFont(font);
    h20.setFont(font);
    so2.setString("SO2");
    co2.setString("CO2");
    hs2.setString("HS2");
    h20.setString("H2O");


    sf::Vector2f particleEmitter = sf::Vector2f(winWid / 2, pEmitY);
    tri.setPosition(sf::Vector2f(particleEmitter.x - tri.getRadius() + 3, particleEmitter.y + tri.getRadius() / 16 - 35));
    tri.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Update particle holder
        if (particles.size() < particleCount) {
            int randChoice = rand() % typeCount;
            float angle = -1.57 + randFloat(-3.14 / 8, 3.14 / 8);
            float mass = masses[randChoice];
            sf::Vector2f randVel = sf::Vector2f(cosf(angle) * initVel / mass, sinf(angle) * initVel / mass);
            Particle p(particleEmitter, randVel, colors[randChoice]);
            particles.push_back(p);
        }

        window.clear();
        window.draw(tri);
        //Update particle positions
        for (Particle& p : particles) {
            if (!p.isVisible()) {
                int randChoice = rand() % typeCount;
                float angle = -1.57 + randFloat(-3.14 / 8, 3.14 / 8);
                float mass = masses[randChoice];
                sf::Vector2f randVel = sf::Vector2f(cosf(angle) * initVel / mass, sinf(angle) * initVel / mass);
                p.setColor(colors[randChoice]);
                p.setVel(randVel);
                p.setPos(particleEmitter);
            }
            else {
                p.updateVel();
                p.updatePos();
            }
            cir.setPosition(p.getPos());
            cir.setFillColor(p.getColor());
            window.draw(cir);
        }
        
        window.draw(legendBg);
        window.draw(so2);
        window.draw(co2);
        window.draw(hs2);
        window.draw(h20);
        window.display();
    }

    return 0;
}