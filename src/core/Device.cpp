#include "Device.hpp"

Pin::Pin(Type t) {
    this->value = { 0b0 };
    this->level = 0;
    this->type = t;
}

Gate::Gate(Type t) {

    this->type = t;
    this->level = 0;
    this->inputs.clear();
    for (int i = 0; i < 2; i++) {
        this->inputs.emplace_back(Pin::Input);
    }

    for (int i = 0; i < this->inputs.size(); i++) {
        this->inputs[i] = Pin(Pin::Input);
    }

    switch (this->type) {
    case (Type::BUFFER):
        for (int i = 0; i < 1; i++) {
            this->inputs.emplace_back(Pin::Input);
        }
    case (AND || OR || OR || XOR || NOT || NAND || XOR || XNOR):
        this->inputs.clear();
        for (int i = 0; i < 2; i++) {
            this->inputs.emplace_back(Pin::Input);
        }
    }
}

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

Gate Gate::Clone(Gate gateToClone) {
    Gate gate(gateToClone.type);
    gate.inputs = gateToClone.inputs;
    gate.output = gateToClone.output;
    gate.GateID = gateToClone.GateID;
    gate.level = gateToClone.level;
    return gate;

}

Wire::Wire() {
    this->value = { 0b0 };
    this->level = 0;
}

void Wire::ConnectIN(const Gate& gateIN) {
    this->value.set(0, gateIN.output.value.test(0));
}

void Wire::ConnectOUT(Gate gateOUT, int inID) const {
    gateOUT.inputs[inID].value.set(0, this->value.test(0));
}