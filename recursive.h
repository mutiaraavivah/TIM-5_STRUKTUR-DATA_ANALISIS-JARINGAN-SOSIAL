#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <iostream>
#include <string>
#include "relation.h"
#include "stringlist.h"
using namespace std;

void recurseShow(Relation& net, const string& curId, StringList& visited, int depth) {
    if (visited.contains(curId)) return;
    visited.push_back(curId);

    User* u = net.getUser(curId);
    if (!u) return;

    for (int i = 0; i < depth; ++i) cout << "  ";
    cout << "- " << u->getName() << " (" << u->getID() << ")\n";

    StringList fr = u->getFriends();
    for (int j = 0; j < fr.size(); ++j) {
        recurseShow(net, fr[j], visited, depth + 1);
    }
}

void showConnectionsRecursive(Relation& net, const string& startId) {
    StringList visited;
    User* start = net.getUser(startId);
    if (!start) {
        cout << "User tidak ditemukan.\n";
        return;
    }

    recurseShow(net, startId, visited, 0);
}

#endif // RECURSIVE_H