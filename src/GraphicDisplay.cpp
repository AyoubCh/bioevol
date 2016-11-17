//
// Created by arrouan on 14/10/16.
//

#include "GraphicDisplay.h"
#include "World.h"
#include "Common.h"
#include "Organism.h"

GraphicDisplay::GraphicDisplay(World* world) {
  world_ = world;

  printf("Windows Size %d %d\n",world->width_*Common::CELL_SCALE+4*Common::SPAN,
         world->height_*Common::CELL_SCALE+2*Common::SPAN);
  _pop_window.create(sf::VideoMode(world->width_*Common::CELL_SCALE+4*Common::SPAN,
                                    world->height_*Common::CELL_SCALE+2*Common::SPAN), "Population");
  _pop_window.setPosition(sf::Vector2i(100, 100));

  _lattice = new sf::RectangleShape(sf::Vector2f(world->width_*Common::CELL_SCALE+
                                                     (world->width_-1)*Common::CELL_SPACE,
                                                 world->height_*Common::CELL_SCALE+
                                                     (world->height_-1)*Common::CELL_SPACE));
  _lattice->setFillColor(sf::Color::Black);
  _lattice->setPosition(Common::SPAN, Common::SPAN);
  _cell = new sf::RectangleShape(sf::Vector2f(Common::CELL_SCALE, Common::CELL_SCALE));

  build_gradients(Common::GRADIENT_SIZE);

}

GraphicDisplay::~GraphicDisplay( void )
{
  delete _lattice;
  _lattice = NULL;
  delete _cell;
  _cell = NULL;
  delete _pop_gradient;
  _pop_gradient = NULL;
}


bool GraphicDisplay::display( void )
{
  if (_pop_window.isOpen())
  {
    /* A) Display graphics ------------------------*/
    _pop_window.clear(sf::Color(50,50,50));
    display_population();
    _pop_window.display();

    _pop_window.setTitle("Population ("+std::to_string(world_->time_)+" timesteps)");

    /* B) Events management -----------------------*/
    sf::Event event;
    while (_pop_window.pollEvent(event))
    {
      /* Window's close event */
      if (event.type == sf::Event::Closed)
      {
        _pop_window.close();
        return false;
      }
    }
    return true;
  }
  return false;
}


bool GraphicDisplay::display_population( void )
{
  _pop_window.draw(*_lattice);
  double min_score  = world_->min_fitness_;
  double max_score  = world_->max_fitness_;

  size_t xcount     = 0;
  size_t ycount     = 0;

  for (size_t x = 0; x < world_->width_; x++)
  {
    ycount = 0;
    for (size_t y = 0; y < world_->height_; y++)
    {
      Organism* org        = world_->grid_cell_[x*world_->width_+y]->organism_;

      _cell->setPosition(sf::Vector2f(Common::SPAN+x*Common::CELL_SCALE+xcount, Common::SPAN+y*Common::CELL_SCALE+ycount));
      if (org != nullptr)
      {
        float score = org->fitness_;
        float value = (score - min_score) / (max_score - min_score);
        _cell->setFillColor(get_color(value < 1.0 ? value : 1.0));
        _pop_window.draw(*_cell);
      }
      ycount += Common::CELL_SPACE;
    }
    xcount += Common::CELL_SPACE;
  }
  _pop_window.draw(_pop_gradient, Common::GRADIENT_SIZE*2, sf::TrianglesStrip);

  return true;
}


void GraphicDisplay::build_gradients(size_t size) {
  /*-----------------------------*/
  /* 1) Create vertices array    */
  /*-----------------------------*/
  _pop_gradient = new sf::Vertex[2*size];

  /*-----------------------------*/
  /* 2) Get lattice size         */
  /*-----------------------------*/
  sf::Vector2f vec = _lattice->getSize();

  /*-----------------------------*/
  /* 3) Initialize coordinates   */
  /*-----------------------------*/
  double x1 = vec.x+2*Common::SPAN;
  double x2 = x1+Common::GRADIENT_SCALE;

  /*-----------------------------*/
  /* 4) Initialize gradient step */
  /*-----------------------------*/
  double step = vec.y/(size-1);

  /*-----------------------------*/
  /* 5) Fill color points        */
  /*-----------------------------*/
  double y = Common::SPAN;
  double value = 1.0;
  for (size_t i = 0; i < 2*size; i += 2)
  {
    _pop_gradient[i]   = sf::Vertex(sf::Vector2f(x1, y), get_color(value));
    _pop_gradient[i+1] = sf::Vertex(sf::Vector2f(x2, y), get_color(value));

    y += step;
    value -= step/vec.y;
  }
}

sf::Color GraphicDisplay::get_color(double value) {
  value = pow(value, 2.0);
  if (value < 0.2)
  {
    return sf::Color((0.1) * 255.0, (0.2) * 255.0, (0.3) * 255.0);
  }
  else if (value >= 0.2 && value < 0.7)
  {
    return sf::Color((0.1 + ((value-0.2) / 0.5)*0.9) * 255.0, (0.2 -
        ((value-0.2) / 0.5)*0.2) * 255.0, (0.3 + ((value-0.2) / 0.5)*0.7) * 255.0);
  }
  else if (value >= 0.7 && value < 0.9)
  {
    return sf::Color((1.0) * 255.0, ((value-0.7) / 0.2) * 255.0, (1.0 -
        (value-0.7) / 0.2) * 255.0);
  }
  else if (value >= 0.9)
  {
    return sf::Color((1.0) * 255.0, (1.0) * 255.0, ((value-0.9) / 0.1) * 255.0);
  }
  return sf::Color(0.0, 255.0, 0.0);
}



