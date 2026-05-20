#include <iostream>
#include "extract.cpp"

int main() {
    Extract extracteur;
    extracteur.lireFichier("ressources/fourmilieres_texte/fourmiliere_0.txt");    
    int ant = extracteur.getants();
    const auto& rooms = extracteur.getRooms();
    const auto& connexions = extracteur.getConnexions();
    return 0;
}
