
// Declaration of the Device class and its member functions
// The Device class is used to create the gates, pins, wires etc. that are used in a circuit/chip

#include <vector>
#include <bitset>
#include <cstdint>

struct Pin {
	enum Type {
		Input, Output, NONE
	};
	std::bitset<1> value;
	Type type;
	int level;
    Pin(Type t);
};

struct Gate {
    enum Type {
        BUFFER, AND, OR, XOR,
        NOT, NAND, NOR, XNOR
    };

    Type type;
    int level;
    int GateID;
    std::vector<Pin> inputs = std::vector<Pin>();
    Pin output = Pin(Pin::Output);

    Gate(Type t);

    void Evaluate();
    Gate Clone(Gate gateToClone);
};

struct Wire {
    std::bitset<1> value;
    int level;

    Wire();

    void ConnectIN(const Gate& gateIN);
    void ConnectOUT(Gate gateOUT, int inID) const;
};