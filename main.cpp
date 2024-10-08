#include <SFML/Graphics.hpp>

int main() {
    // Créer une fenêtre SFML avec une taille de 200x200 et un titre "SFML works!"
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    // Créer un cercle de rayon 100 et définir sa couleur de remplissage en vert
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Boucle principale de la fenêtre
    while (window.isOpen()) {
        sf::Event event;

        // Gérer les événements de la fenêtre
        while (window.pollEvent(event)) {
            // Fermer la fenêtre si l'événement de fermeture est déclenché
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Effacer la fenêtre avant de dessiner
        window.clear();
        
        // Dessiner le cercle
        window.draw(shape);

        // Afficher la fenêtre avec les éléments dessinés
        window.display();
    }

    return 0;
}

