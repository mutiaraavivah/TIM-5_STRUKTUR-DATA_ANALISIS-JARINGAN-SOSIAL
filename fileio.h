#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <string>
#include "relation.h"
using namespace std;

void saveNetworkData(Relation& net, const string& filename) {
	ofstream out(filename.c_str());
	if (!out) {
		cout << "Gagal menyimpan file.\n";
		return;
	}
	
	for (int i=0; i<net.getTotalUsers(); ++i) {
		User* u = net.getUserByIndex(i);
		if (u) out << u->getID() << ";" << u->getName() << "\n";
	}
	cout << "Data berhasil disimpan ke file: " << filename << endl;
}

void loadNetworkData(Relation& net, const string& filename) {
	ifstream in(filename.c_str());
	if (!in) {
		cout << "File tidak ditemukan.\n";
		return;
	}
	string line;
	while (getline(in, line)) {
		size_t sep = line.find(';');
		if (sep == string::npos) continue;
		string id = line.substr(0, sep);
		string name = line.substr(sep + 1);
		net.addUser(id, name);
	}
	cout << "Data berhasil dimuat dari file: " << filename << endl;
}

#endif
