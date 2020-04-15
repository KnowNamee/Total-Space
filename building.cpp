#include "building.h"

Building::Building() {}

int64_t Building::GetToolsIncome() const { return income_.tools; }
int64_t Building::GetBatteriesIncome() const { return income_.batteries; }
