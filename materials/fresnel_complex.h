#ifndef MATERIALS_FRESNEL_COMPLEX_H
#define MATERIALS_FRESNEL_COMPLEX_H

//TODO: Implement complex Fresnel

class FresnelComplex : public Fresnel {
   public:
    FresnelComplex() : Fresnel(FresnelType::COMPLEX) {}
};

#endif  // MATERIALS_FRESNEL_COMPLEX_H
