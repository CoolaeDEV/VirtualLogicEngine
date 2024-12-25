
// Initalization of the Device class and its methods
// The Device class is used to create the gates, pins, wires etc. that are used in a circuit/chip

#include "Device.hpp"


// Pin Constructer
Pin::Pin(Type t) {
    this->value = { 0b0 };
    this->level = 0;
    this->type = t;
}


// Gate Constructer ( initalizes states, Sets Type to (t), Checks if the type of Gate is a buffer and if it is then input array with be 1 deep,
// else it will be 2 deep)
Gate::Gate(Type t) {

    this->type = t;
    this->level = 0;
    this->inputs.clear();

    switch (this->type) {
    case (Type::BUFFER):
        for (int i = 0; i < 1; i++) {
            this->inputs.emplace_back(Pin::Input);
        }
    case (AND || OR || OR || XOR || NOT || NAND || XOR || XNOR):
        for (int i = 0; i < 2; i++) {
            this->inputs.emplace_back(Pin::Input);
        }
    }
}

// Evaluate the Gate based on its type
void Gate::Evaluate() {
    switch (this->type) {
    case(Gate::BUFFER):
        this->output.value.set(0, this->inputs[0].value.test(0));
        break;
    case (Gate::AND):
        if ((this->inputs[0].value.test(0)) && (this->inputs[1].value.test(0))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case (Gate::OR):
        if ((this->inputs[0].value.test(0)) || (this->inputs[1].value.test(0))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case(Gate::XOR):
        if (!(this->inputs[0].value.test(0)) != !(this->inputs[1].value.test(0))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case(Gate::NOT):
        if (!(this->inputs[0].value.test(0))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case(Gate::NAND):
        if (!((this->inputs[0].value.test(0)) && (this->inputs[1].value.test(0)))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case(Gate::NOR):
        if (!((this->inputs[0].value.test(0)) && (this->inputs[1].value.test(0)) == true)) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    case(Gate::XNOR):
        if (!(this->inputs[0].value.test(0)) == !(this->inputs[1].value.test(0))) {
            this->output.value.set(0, true);
        }
        else {
            this->output.value.set(0, false);
        }
        break;
    }
}


// Clones a gate and returns the clone
Gate Gate::Clone(Gate gateToClone) {
    Gate gate(gateToClone.type);
    gate.inputs = gateToClone.inputs;
    gate.output = gateToClone.output;
    gate.GateID = gateToClone.GateID;
    gate.level = gateToClone.level;
    return gate;

}

//Wire Constructer
Wire::Wire() {
    this->value = { 0b0 };
    this->level = 0;
}

// Connects the output of a gate to the input of the wire
void Wire::ConnectIN(const Gate& gateIN) {
    this->value.set(0, gateIN.output.value.test(0));
}

// Connects the input of a gate to the output of the wire
void Wire::ConnectOUT(Gate gateOUT, int inID) const {
    gateOUT.inputs[inID].value.set(0, this->value.test(0));
}