#include "headers/Wire.h"  
#include "headers/Gate.h"  

Wire::Wire(Gate* from, Gate* to, int inputIndex, uint32_t delay) : delayTicks(delay), value(Value::LOW), dirty(false) {  
   if (from && to != nullptr) {  
       std::vector<Wire*>& toInputs = const_cast<std::vector<Wire*>&>(to->getInputs());
       toInputs.push_back(const_cast<Wire*>(from->getOutput()));
   }  
}

void Wire::setValue(Value newValue) {  
   if (value != newValue) {  
       value = newValue;  
       markDirty();  
   }  
}  

Wire::Value Wire::getValue() const {  
   return value;  
}  

void Wire::markDirty() {  
   dirty = true;  
}  

void Wire::clearDirty() {  
   dirty = false;  
}  

bool Wire::isDirty() const {  
   return dirty;  
}  

void Wire::attachGate(Gate* gate) {  
   attachedGates.push_back(gate);  
}  

const std::vector<Gate*>& Wire::getAttachedGates() const {  
   return attachedGates;  
}