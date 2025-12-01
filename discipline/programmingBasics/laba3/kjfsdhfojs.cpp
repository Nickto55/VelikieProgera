#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

const double PI = 3.14159265358979323846;

struct Matrix3x3 {
    float m[3][3];

    static Matrix3x3 identity() {
        Matrix3x3 mat{};
        for (int i = 0; i < 3; i++) mat.m[i][i] = 1;
        return mat;
    }

    static Matrix3x3 translation(float dx, float dy) {
        Matrix3x3 mat = identity();
        mat.m[0][2] = dx;
        mat.m[1][2] = dy;
        return mat;
    }

    static Matrix3x3 rotation(float angleDeg) {
        Matrix3x3 mat = identity();
        float rad = angleDeg * PI / 180.0;
        float c = cos(rad);
        float s = sin(rad);
        mat.m[0][0] = c; mat.m[0][1] = -s;
        mat.m[1][0] = s; mat.m[1][1] = c;
        return mat;
    }

    static Matrix3x3 scaling(float s) {
        Matrix3x3 mat = identity();
        mat.m[0][0] = s;
        mat.m[1][1] = s;
        return mat;
    }
};

sf::Vector2f transformPoint(const sf::Vector2f& p, const Matrix3x3& mat) {
    float x = mat.m[0][0]*p.x + mat.m[0][1]*p.y + mat.m[0][2];
    float y = mat.m[1][0]*p.x + mat.m[1][1]*p.y + mat.m[1][2];
    return sf::Vector2f(x, y);
}

Matrix3x3 multiply(const Matrix3x3& a, const Matrix3x3& b) {
    Matrix3x3 r{};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                r.m[i][j] += a.m[i][k] * b.m[k][j];
    return r;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Вращение вокруг центра");
    window.setFramerateLimit(60);

    std::vector<sf::Vector2f> basePoints = {
        {0.f, 0.f}, {50.f, 80.f}, {130.f, 60.f},
        {150.f, 0.f}, {100.f, -60.f}, {40.f, -40.f}
    };

    sf::ConvexShape hex;
    hex.setPointCount(basePoints.size());
    for (size_t i = 0; i < basePoints.size(); ++i)
        hex.setPoint(i, basePoints[i]);

    hex.setFillColor(sf::Color::Transparent);
    hex.setOutlineColor(sf::Color::Black);
    hex.setOutlineThickness(2.f);

    // Вычисляем центр
    sf::Vector2f center(0.f, 0.f);
    for (auto& p : basePoints)
        center += p;
    center.x /= basePoints.size();
    center.y /= basePoints.size();

    float angle = 0.f;
    float scale = 1.f;
    sf::Vector2f position(400, 300); // начальная позиция фигуры на экране

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            position.x -= 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            position.x += 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            position.y -= 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            position.y += 3;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            angle -= 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            angle += 2;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            scale *= 1.01f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            scale *= 0.99f;

        // Создание матриц преобразования
        Matrix3x3 toOrigin = Matrix3x3::translation(-center.x, -center.y); // сдвиг к (0,0)
        Matrix3x3 rot = Matrix3x3::rotation(angle);
        Matrix3x3 scl = Matrix3x3::scaling(scale);
        Matrix3x3 back = Matrix3x3::translation(center.x, center.y);       // вернуть обратно
        Matrix3x3 move = Matrix3x3::translation(position.x, position.y);   // перенос на экран

        // Итоговая матрица преобразования
        Matrix3x3 finalMat = multiply(move, multiply(back, multiply(rot, multiply(scl, toOrigin))));

        // Применяем матрицу преобразования ко всем точкам фигуры
        for (size_t i = 0; i < basePoints.size(); ++i)
            hex.setPoint(i, transformPoint(basePoints[i], finalMat));

        // Не нужно вызывать setPosition, потому что это уже учтено в матрице.
        // hex.setPosition(position); // УДАЛЁН!

        window.clear(sf::Color::White);
	window.draw(hex);
        window.display();
    }

    return 0;
}
