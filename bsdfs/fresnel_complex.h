#ifndef BSDFS_FRESNEL_COMPLEX_H
#define BSDFS_FRESNEL_COMPLEX_H

//TODO: Implement complex Fresnel

class FresnelComplex : public Fresnel {
   public:
    FresnelComplex() : Fresnel(FresnelType::COMPLEX) {}
};

#endif  // BSDFS_FRESNEL_COMPLEX_H
