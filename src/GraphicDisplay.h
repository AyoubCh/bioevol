//
// Created by arrouan on 14/10/16.
//

#ifndef PDC_EVOL_MODEL_GRAPHICDISPLAY_H
#define PDC_EVOL_MODEL_GRAPHICDISPLAY_H


#include <SFML/Graphics.hpp>
#include <cstddef>
#include "World.h"

class GraphicDisplay {
 public:
    GraphicDisplay(World* world);
    ~GraphicDisplay();

    World* world_;

    sf::RenderWindow   _pop_window;   /*!< Population render window   */
    sf::RectangleShape* _lattice;      /*!< Lattice shape              */
    sf::RectangleShape* _cell;         /*!< Cell shape                 */
    sf::Vertex*         _pop_gradient; /*!< Population color gradient  */

    void build_gradients(size_t size);

    sf::Color get_color(double value);

    bool display(void);

    bool display_population(void);
};


#endif //PDC_EVOL_MODEL_GRAPHICDISPLAY_H
