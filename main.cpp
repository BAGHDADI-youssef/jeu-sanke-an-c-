#include <SFML/Graphics.hpp>

int main() {
    // Cr�er une fen�tre SFML avec une taille de 200x200 et un titre "SFML works!"
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    // Cr�er un cercle de rayon 100 et d�finir sa couleur de remplissage en vert
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Boucle principale de la fen�tre
    while (window.isOpen()) {
        sf::Event event;

        // G�rer les �v�nements de la fen�tre
        while (window.pollEvent(event)) {
            // Fermer la fen�tre si l'�v�nement de fermeture est d�clench�
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Effacer la fen�tre avant de dessiner
        window.clear();
        
        // Dessiner le cercle
        window.draw(shape);

        // Afficher la fen�tre avec les �l�ments dessin�s
        window.display();
    }

    return 0;
}

