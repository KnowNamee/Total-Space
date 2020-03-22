#ifndef BUILDING_H
#define BUILDING_H
class Planet;

class Building
{
public:
  Building();
private:
  Planet* parent;
};

#endif // BUILDING_H
