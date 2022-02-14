//
// Created by Debraj Ray on 2022-02-13.
//

#include "AStarOpenList.h"

float AStarOpenList::leftChild(int parentIndex) {
    return fscoreHeap[2*parentIndex + 1].getFScore();
}

float AStarOpenList::rightChild(int parentIndex) {
    return fscoreHeap[2*parentIndex + 2].getFScore();
}

void AStarOpenList::sink(int k) {
    int leftChildIdx, rightChildIdx, minChildIdx;
    while(2*k + 1 <size) {
        leftChildIdx = 2*k + 1;
        rightChildIdx = 2*k + 2;
        if (rightChildIdx < size) {
            minChildIdx = rightChildIdx;
            if (leftChild(k) < rightChild(k)) {
                minChildIdx = leftChildIdx;
            }
            if(fscoreHeap[minChildIdx].getFScore() < fscoreHeap[k].getFScore()){
                exchange(minChildIdx, k);
                k = minChildIdx;
            }
        } else {
            if (leftChild(k) < fscoreHeap[k].getFScore()) {
                exchange(leftChildIdx, k);
                k = leftChildIdx;
            } else {
                break;
            }
        }
    }
}

void AStarOpenList::swim(int k) {
    int parent_index;
    while (k>=1) {
        parent_index = (k - 1) / 2;
        if (fscoreHeap[k].getFScore() < fscoreHeap[parent_index].getFScore()) {
            exchange(k, parent_index);
            k = parent_index;
        } else {
            break;
        }
    }
}

void AStarOpenList::insert(node_& node) {
    fscoreHeap.push_back(node);
    size++;
    swim(size-1);
    openList.insert(node);
    node.heap_idx = size;
}

void AStarOpenList::exchange(int idx1, int idx2) {
    node_ temp = fscoreHeap[idx1];
    fscoreHeap[idx1] = fscoreHeap[idx2];
    fscoreHeap[idx2] = temp;

    fscoreHeap[idx1].heap_idx = idx1;
    fscoreHeap[idx2].heap_idx = idx2;
}

node_ AStarOpenList::removeMinimum() {
    node_ fscore = fscoreHeap[0];
    fscoreHeap.erase(fscoreHeap.begin());
    openList.erase(fscore);
    size--;
    return fscore;
}

float AStarOpenList::peekMinimum() {
    return fscoreHeap[0].getFScore();
}

bool AStarOpenList::isEmpty() {
    return size == 0;
}

bool AStarOpenList::isPresent(node_& n) {
    return openList.find(n) != openList.end();
}

bool AStarOpenList::updateIfBetterPath(node_& n, float gvalue) {
    auto t = openList.find(n);
    if (gvalue < t->g) {
        n.computeF(gvalue, t->h);
        fscoreHeap[t->heap_idx] = n;
        swim(t->heap_idx);
        openList.erase(n);
        openList.insert(n);
        return true;
    }
    return false;
}
