#include <iostream>
#include <vector>
#include <unordered_map>
#include "../headers/room.hpp"
#include "../headers/algorithm.hpp"
#include "../headers/ants.hpp"
#include "../headers/extract.hpp"
#include "../headers/link.hpp"

using namespace std;

int main() {
    Extract extract;
    extract.lireFichier("ressources/fourmilieres_texte/fourmiliere_1.txt");
    int antCount = extract.getants();
    cout << "Number of ants: " << antCount << endl;
    Link link;
    link.buildFromExtract(extract);

    Room* start = link.getSv();
    Room* end = link.getSd();

    Algorithm algo(antCount, 0, antCount);
    algo.runAndSimulate(start, end);

    return 0;
}
