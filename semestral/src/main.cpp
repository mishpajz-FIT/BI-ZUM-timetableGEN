#include "Custom/FITCTUFileImporter.h"
#include "Custom/StdinAdjuster.h"
#include "Custom/StdoutOutputter.h"
#include "evolution.h"

#include <iostream>
#include <vector>

int main() {

    CS_FITCTUFileImporter importer("examples/example1.txt");
    Semester semester = importer.import();

    StdinAdjuster adjuster;
    Priorities priorities = adjuster(semester);

    Evolution evolution(semester, priorities);
    std::vector<EvolutionResult> result = evolution.evolve();

    CS_StdoutOutputter outputter;
    outputter.output(result);

    return 0;
}