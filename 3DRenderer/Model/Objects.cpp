#include "Objects.h"
double renderer::RGB::GetR() const {
    return val_(0);
}
void renderer::RGB::SetR(double val) {

    val_(0) = val;
}
double renderer::RGB::GetG() const {
    return val_(1);
}
void renderer::RGB::SetG(double val) {
    val_(1) = val;
}
double renderer::RGB::GetB() const {
    return val_(2);
}
void renderer::RGB::SetB(double val) {
    val_(2) = val;
}
