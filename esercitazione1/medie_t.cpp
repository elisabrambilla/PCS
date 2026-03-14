#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char *argv[])
{
    if(argc != 2) {
        cout << "C'è un errore nell'inserimento del nome del file \n";
    return 0;
    }
 
    string filename = argv[1];
    ifstream ifs(filename);
    if (ifs.is_open()) {
        for (int i=0; i<=2; i++) {
            string citta;
            double t1,t2,t3,t4;
            ifs >> citta >> t1 >> t2 >> t3 >> t4;
            double media_temp = (t1 + t2 + t3 + t4)/4;
            cout << citta << ": " << media_temp << "\n";
        }
        return 0;    
    }
    else {
        cout << "Errore: mancata apertura del file \n";
        return 1;
    }
}