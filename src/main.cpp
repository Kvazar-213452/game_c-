#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

// Клас частки
class Particle {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float rotationSpeed; // Швидкість обертання

    Particle(float x, float y) {
        shape.setSize(sf::Vector2f(5.f, 5.f)); // Менші частки
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(x, y);
        velocity = sf::Vector2f((rand() % 5 - 2) * 0.033f, (rand() % 5 - 2) * 0.033f);  // Рандомний напрямок, зменшено в 3 рази
        lifetime = 1.f;  // Час життя частки
        rotationSpeed = (rand() % 10 - 5) * 5.f;  // Рандомна швидкість обертання
    }

    // Оновлення частки
    void update(float deltaTime) {
        shape.move(velocity);
        lifetime -= deltaTime;  // Зменшуємо час життя
        shape.setFillColor(sf::Color(255, 255, 0, static_cast<int>(255 * lifetime)));  // Зменшуємо прозорість
        shape.rotate(rotationSpeed * deltaTime);  // Обертання частки
    }

    bool isAlive() const {
        return lifetime > 0.f;  // Частка живе, поки її час життя більше 0
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Rotating Particles");

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Blue);
    player.setPosition(375.f, 275.f);

    std::vector<Particle> particles;
    float playerSpeed = 0.5f;  // Зменшена швидкість гравця до 0.5

    sf::Clock clock;  // Для обчислення часу
    bool createParticles = false; // Прапор для контролю часток
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Обробка руху гравця
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.move(-playerSpeed, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.move(playerSpeed, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player.move(0.f, -playerSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player.move(0.f, playerSpeed);

        // Створюємо частки рідше (в два рази менше)
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && rand() % 2 == 0) {
            particles.push_back(Particle(player.getPosition().x + 25.f, player.getPosition().y + 25.f));
        }

        // Оновлення часток
        float deltaTime = clock.restart().asSeconds();
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }

        // Видалення мертвих часток
        particles.erase(std::remove_if(particles.begin(), particles.end(),
                                       [](const Particle& p) { return !p.isAlive(); }),
                        particles.end());

        // Очищаємо вікно
        window.clear();

        // Малюємо всі частки
        for (const auto& particle : particles) {
            window.draw(particle.shape);
        }

        // Малюємо гравця
        window.draw(player);

        // Виводимо все на екран
        window.display();
    }

    return 0;
}
