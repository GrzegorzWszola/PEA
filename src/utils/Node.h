#pragma once

class Node {
    private:
        int id;
        int x;
        int y;
    public:
        Node() : id(-1), x(0), y(0) {}
        Node(int idInput, int xInput, int yInput)
            : id(idInput), x(xInput), y(yInput) {}
        int getX() const { return x; }
        int getY() const { return y; }
        int getId() const { return id; }
};