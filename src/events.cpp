#include "events.h"

void process_events(sf::Window& window){
    while (const std::optional event = window.pollEvent()){
        if (event->is<sf::Event::Closed>()){
            window.close();
        }
    }
}
