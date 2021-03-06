#ifndef __ComplexShortArray_h__
#define __ComplexShortArray_h__

#include "ShortArray.h"
#include "basicmaths.h"
class ComplexIntArray;

/**
* A structure defining a fixed point complex number as two members of type int16_t.
*/
struct ComplexShort {
  /**
  The real part of the complex number.
  */
  int16_t re;
  
  /**
  The imaginary part of the complex number.
  */
  int16_t im;
  
  /**
  Get the magnitude of the complex number.
  Computes and returns the magnitude of the complex number.
  @return The magnitude of the complex number.
  */
  int16_t getMagnitude(){
  #ifdef ARM_CORTEX
    int16_t out;
    int16_t in[2] = {re, im};
    arm_cmplx_mag_q15(in, &out, 1);
  // function above returns 2.14, so we shift it back to 1.15
  // but first we check we do not lose data.
  // TODO: make sure it saturates
    out = out >> 1;
    return out;
  #else
    #error TODO
    return int16_t(sqrtf(re*re+im*im) + 0.5);
  #endif
  }
  
  /**
  Get the phase of the complex number.
  Computes and returns the phase of the complex number.
  @return The phase of the complex number.
  */  
  float getPhase(){
    return atan2(im,re);
  }
  
  /**
  Set the phase of the complex number.
  Set the phase of the complex number, keeping the magnitude unaltered.
  @param phase The new phase of the complex number
  */
  void setPhase(float phase){
    int16_t magnitude=getMagnitude();
    setPolar(magnitude, phase);
  }
  
  /**
  Set the magnitude of the complex number.
  Set the magnitude of the complex number, keeping the phase unaltered.
  @param magnitude The new magnitude of the complex number
  */
  void setMagnitude(int16_t magnitude){
    float phase=getPhase();
    setPolar(magnitude, phase);
  }
  
  /**
  Set magnitude and phase of the complex number.
  @param magnitude The new magnitude of the complex number
  @param phase The new phase of the complex number
  */
  void setPolar(int16_t magnitude, float phase){
    re=(int16_t)(magnitude*cosf(phase) + 0.5);
    im=(int16_t)(magnitude*sinf(phase) + 0.5);
  }
};

class ComplexShortArray {
private:
  ComplexShort* data;
  unsigned int size;
public:
  /**Constructor

    Initializes size to 0.
  */
  ComplexShortArray() :
    data(NULL), size(0) {}
  
  /**
    Constructor.
      
    @param array A pointer to an array of ComplexShort
    @param size The length of the rray
  */
  ComplexShortArray(ComplexShort* array, unsigned int size) :
    data(array), size(size) {}
      
  /** 
    The real part of an element of the array.
      
    @param i The index of the element
    @return The real part of the element
  */
  int16_t re(const int i){
    return data[i].re;
  }
  
  /**
    The imaginary part of an element of the array.
    @param i The index of the element
    @return The imaginary part of the element
  */
  int16_t im(const int i){
    return data[i].im;
  }
  
  void clear(){
    setAll(0);
  }

  /**
    The magnitude of an element of the array.
    @param i The index of the element
    @return The magnitude of the element
  */  
  int16_t mag(const int i);
  
  /**
    The magnitudes of the elements of the array.
    @param[out] destination The array where the magnitude values will be stored.
  */  
  void getMagnitudeValues(ShortArray destination);
  
  /**
    The magnitude squared of an element of the array.
    @param i The index of the element
    @return The magnitude squared of the element
  */  
  int16_t mag2(const int i);
  
  /**
    The squared magnitudes of the elements of the array.
    @param[out] destination The array where the magnitude squared values will be stored.
  */  
  void getMagnitudeSquaredValues(ShortArray destination);
  
   /**
    The complex conjugate values of the element of the array.
    @param[out] destination The array where the complex conjugate values will be stored.
  */  
  void getComplexConjugateValues(ComplexShortArray destination);
  
   /**
    Complex dot product between arrays.
    @param[in] operand2 The second operand of the dot product
    @param[out] result The array where the result of the dot product is stored 
  */  
  void complexDotProduct(ComplexShortArray operand2, ComplexShort& result);

   /**
    Complex by complex multiplication between arrays.
    @param[in] operand2 The second operand of the multiplication
    @param[out] result The array where the result of the multiplication is stored 
  */  
  void complexByComplexMultiplication(ComplexShortArray operand2, ComplexShortArray result);
  
   /**
    Complex by real multiplication between arrays.
    @param[in] operand2 The second operand of the multiplication
    @param[out] result The array where the result of the multiplication is stored 
  */ 
  void complexByRealMultiplication(ShortArray operand2, ComplexShortArray result);

  /**
   * Element-wise sum between complex arrays.
   * Sets each element in **destination** to the complex sum of the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the sum
   * @param[out] destination the destination array
  */
  void add(ComplexShortArray operand2, ComplexShortArray destination);

  /**
   * In-place element-wise sum between complex arrays.
   * Adds each element of **operand2** to the corresponding element in the array.
   * @param operand2 second operand for the sum
  */
  void add(ComplexShortArray operand2);

  /**
   * Element-wise difference between complex arrays.
   * Sets each element in **destination** to the complex difference between the corresponding element of the array and **operand2**
   * @param[in] operand2 second operand for the subtraction
   * @param[out] destination the destination array
  */
  void subtract(ComplexShortArray operand2, ComplexShortArray destination);

  /**
   * In-place element-wise difference between complex arrays.
   * Sutracts each element of **operand2** from the corresponding element in the array.
   * @param operand2 second operand for the sum
  */
  void subtract(ComplexShortArray operand2);

  unsigned int getSize() const{
    return size;
  }
  
  /**
    The value of the element with the maximum magnitude in the array.
    @return The maximum magnitude value in the array.
  */ 
  int16_t getMaxMagnitudeValue();
  
  /**
    The index of the element with the maximum magnitude in the array.
    @return The index of the element with the maximum magnitude in the array.
  */ 
  unsigned int getMaxMagnitudeIndex();
  
  /**
   * A subset of the array.
   * Returns an array that points to subset of the memory used by the original array.
   * @param[in] offset the first element of the subset.
   * @param[in] length the number of elments in the new ComplexShortArray.
   * @return the newly created ComplexShortArray.
   * @remarks no memory is allocated by this method. The memory is still shared with the original array.
   * The memory should not be de-allocated elsewhere (e.g.: by calling ComplexShortArray::destroy() on the original ComplexShortArray) 
   * as long as the ComplexShortArray returned by this method is still in use.
   * @remarks Calling ComplexShortArray::destroy() on a ComplexShortArray instance created with this method might cause an exception.
  */
  ComplexShortArray subArray(unsigned int offset, unsigned int length);
  
  /** Get the real part of the elements of the array.
   * @param[out] buf The array where the real part will be stored.
   */   
  void getRealValues(ShortArray buf);
  
  /** Get the imaginary part of the elements of the array.
   * @param[out] buf The array where the imaginary part will be stored.
  */   
  void getImaginaryValues(ShortArray buf);
  
  /** Array by scalar multiplication.
   * @param factor The value by which all the elements of the array are multiplied.
   */
  void scale(int16_t factor);
  
  /**
   * Allows to index the array using array-style brackets.
   * @param index The index of the element
   * @return the Value of the <code>index</code> element of the array
   * Example usage:
   * @code
   * int size=1000;
   * int16_t content[size]; 
   * ComplexShortArray complexShortArray(content, size);
   * for(int n=0; n<size; n+=2){//now the ComplexShortArray can be indexed as if it was an array
   *   content[n]==complexShortArray[n/2].re; 
   *   content[n+1]==complexShortArray[n/2].im;
   * }
   * @endcode
  */
  ComplexShort& operator [](const int index){
    return data[index];
  }
  
  /**
   * Allows to index the array using array-style brackets.
   * 
   * <code>const</code> version of operator[]
  */
  ComplexShort& operator [](const int i) const{
    return data[i];
  }

  /*
   * Casting operator to ComplexShort*
   * @return A ComplexShort* pointer to the data stored in the ComplexShortArray
  */  
  operator ComplexShort*() {
    return data;
  }
  /**
   * Get the data stored in the ComplexShortArray.
   * @return a ComplexShort* pointer to the data stored in the ComplexShortArray
  */
  ComplexShort* getData(){
    return data;
  }

  /**
   * Compares two arrays.
   * Performs an element-wise comparison of the values contained in the arrays.
   * @param other the array to compare against.
   * @return <code>true</code> if the arrays have the same size and the value of each of the elements of the one 
   * match the value of the corresponding element of the other, or <code>false</code> otherwise.
  */
  bool equals(const ComplexShortArray other) const{
    if(size!=other.getSize()){
      return false;
    }
    for(int n=0; n<size; ++n){
      if(data[n].re!=other[n].re || data[n].im!=other[n].im){
        return false;
      }
    }
    return true;
  }
  
  /**
   * Creates a new ComplexShortArray.
   * Allocates size*sizeof(int16_t) bytes of memory and returns a ComplexShortArray that points to it.
   * @param size The size of the new ComplexShortArray.
   * @return A ComplexShortArray which **data** point to the newly allocated memory and <code>size</code> is initialized to the proper value.
   * @remarks A ComplexShortArray created with this method has to be destroyed invoking the ComplexShortArray::destroy() method.
  */
  static ComplexShortArray create(unsigned int size);

  /**
   * Destroys a ComplexShortArray created with the create() method.
   * @param array The ComplexShortArray to be destroyed.
   * @remarks The ComplexShortArray object passed as an argument should not be used again after invoking this method.
   * @remarks A ComplexShortArray object that has not been created by the ComplexShortArray::create() method might cause an exception if passed as an argument to this method.
  */
  static void destroy(ComplexShortArray);

  /**
   * Copies the content of an array into another array.
   * @param[in] source The source array
  */
  void copyFrom(ShortArray source);

  /**
   * Copies the content of a ShortArray into the ComplexShortArray.
   * @param[in] source The source array
   * @remarks The size of the ShortArray must be twice the size of the ComplexShortArray.
  */
  void copyFrom(ComplexShortArray source);
  
  /**
   * Copies an array of ComplexShort into the array.
   * @param[in] source A pointer to the beginning of the portion of memory to read from.
   * @param[in] length Number of samples to copy.
  */
  void copyFrom(ComplexShort* source, unsigned int length);
  
  /**
   * Copies the content of the ComplexShortArray into a ShortArray.
   * @param[out] destination The destination array
   * @remarks The size of the ShortArray must be twice the size of the ComplexShortArray.
  */
  void copyTo(ShortArray destination);

  /**
   * Copies the content of the array to another array.
   * @param[out] destination The destination array
  */
  void copyTo(ComplexShortArray destination);
  
  /**
   * Copies an array of ComplexShort into the array.
   * @param[in] destination A pointer to the beginning of the portion of memory to write to.
   * @param[in] length Number of samples to copy.
  */
  void copyTo(ComplexShort* destination, int length);


  /**
   * Set all the elements in the array.
   * @param[in] value All the elements are set to this value.
  */
  void setAll(ComplexShort value);
  
  /**
   * Set all the elements in the array.
   * @param[in] value The real and imaginary parts of all the elements in the array are set to this value.
  */
  void setAll(int16_t value);
  
  /**
   * Set all the elements in the array.
   * @param[in] valueRe The real part of every element of the the array will be set to this value.
   * @param[in] valueIm The imaginary pary of rvery element of the the array will be set to this value.
  */
  void setAll(int16_t valueRe, int16_t valueIm);

  /**
   * Set all the elements in the array using polar coordinates.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] phase An array containing the phases.
  */
  void setPolar(ShortArray magnitude, ShortArray phase);
  
  /**
   * Set a range of elements in the array using polar coordinates.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */  
  void setPolar(ShortArray magnitude, ShortArray phase, int offset, int count);

  /**
   * Set the phase of the elements of the array, leaving the magnitude unchanged.
   * @param[in] phase An array containing the phases.
  */
  void setPhase(ShortArray phase);

  /**
   * Set the phase of a range of elements of the array, leaving the magnitude unchanged.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */
  void setPhase(ShortArray phase, int offset, int count);
  
  /**
   * Set the phase of the elements of an array, using the magnitude from the current array.
   * @param[in] phase An array containing the phases.
   * @param[out] destination The destination array.
  */
  void setPhase(ShortArray phase, ComplexShortArray destination);
  
  /**
   * Set the phase of a range of the elements of an array, using the magnitude from the current array.
   * 
   * The <code>offset</code> and <code>count</code> specified apply to both the source and <code>destination</code> arrays. 
   * Elements of the <code>destination</code> array outside this range will not be affected.
   * @param[in] phase An array containing the phases.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
   * @param[out] destination The destination array.
  */
  void setPhase(ShortArray phase, int offset, int count, ComplexShortArray destination);


  /**
   * Set the magnitude of the elements of the array, leaving the phase unchanged.
   * @param[in] magnitude An array containing the magnitudes.
  */
  void setMagnitude(ShortArray magnitude);

/**
   * Set the magnitude of a range of elements of the array, leaving the phase unchanged.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
  */
  void setMagnitude(ShortArray magnitude, int offset, int count);
  
  /**
   * Set the magnitude of the elements of an array, using the phase from the current array.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[out] destination The destination array.
  */
  void setMagnitude(ShortArray magnitude, ComplexShortArray destination);
  
  /**
   * Set the magnitude of a range of the elements of an array, using the phases from the current array.
   * 
   * The <code>offset</code> and <code>count</code> specified apply to both the source and <code>destination</code> arrays. 
   * Elements of the <code>destination</code> array outside this range will not be affected.
   * @param[in] magnitude An array containing the magnitudes.
   * @param[in] offset First element to set
   * @param[in] count Number of elements to set
   * @param[out] destination The destination array.
  */
  void setMagnitude(ShortArray magnitude, int offset, int count, ComplexShortArray destination);
};

/**
* A structure defining a fixed point complex number as two members of type int32_t.
*/
struct ComplexInt {
  /**
  The real part of the complex number.
  */
  int32_t re;
  
  /**
  The imaginary part of the complex number.
  */
  int32_t im;
  
  /**
  Get the magnitude of the complex number.
  Computes and returns the magnitude of the complex number.
  @return The magnitude of the complex number.
  */
};

class ComplexIntArray {
private:
  ComplexInt* data;
  unsigned int size;
public:
  /**Constructor

    Initializes size to 0.
  */
  ComplexIntArray() :
    data(NULL), size(0) {}
  
  /**
    Constructor.
      
    @param array A pointer to an array of ComplexShort
    @param size The length of the rray
  */
  ComplexIntArray(ComplexInt* array, unsigned int size) :
    data(array), size(size) {}

  static ComplexIntArray create(unsigned int size){
    return ComplexIntArray(new ComplexInt[size], size);
  }

  static void destroy(ComplexIntArray array){
    delete array.data;
  }

  ComplexInt& operator [](const int index){
    return data[index];
  }
  
  ComplexInt& operator [](const int i) const{
    return data[i];
  }

  void add(ComplexIntArray operand2, ComplexIntArray destination){
    //ASSERT(operand2.size == size && destination.size >= size, "Arrays size mismatch");
#ifdef ARM_CORTEX
    arm_add_q31((int32_t*)data, (int32_t*)operand2.getData(), (int32_t*)destination.getData(), size*2);
#else
    for(int n=0; n<size; n++){
      destination[n].re = data[n].re + operand2[n].re;
      destination[n].im = data[n].im + operand2[n].im;
    }
#endif /* ARM_CORTEX */  
  }

  void add(ComplexIntArray operand2){
    add(operand2, *this);
  }
  
  void copyFrom(ComplexShortArray operand2){
#ifdef ARM_CORTEX
    arm_q15_to_q31((int16_t*)operand2.getData(), (int32_t*)data, size * 2);
#else
    for(int n = 0; n < size; ++n){
      data[n].re = operand2[n].re;
      data[n].im = operand2[n].im ;
    }
#endif
  }
  
  void copyTo(ComplexShortArray operand2){
#ifdef ARM_CORTEX
    arm_q31_to_q15((int32_t*)data, (int16_t*)operand2.getData(), size * 2);
#else
    for(int n = 0; n < size; ++n){
      data[n].re = (int16_t)operand2[n].re;
      data[n].im = (int16_t)operand2[n].im ;
    }
#endif
  }
  
  operator ComplexInt*() {
    return data;
  }

  ComplexInt* getData(){
    return data;
  }

  unsigned int getSize(){
    return size;
  }
};
#endif // __ComplexShortArray_h__
