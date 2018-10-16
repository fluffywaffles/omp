#include <assert.h>
#include <cpuid.h>
#include <stdio.h>

/**
 * Known cpuid parameters and extended parameters.
 */
#define  HIGHEST_FUNCTION_PARAMETER                0
#define  VENDOR_ID                                 0
#define  PROCESSOR_INFO                            1
#define  CACHE_INFO                                2
#define  SERIAL_NUMBER                             3
#define  PROCESSOR_TOPOLOGY                        4
#define  EXTENDED_FEATURES                         7
#define  EXTENDED_HIGHEST_FUNCTION_PARAMETER       0x80000000
#define  EXTENDED_PROCESSOR_INFO                   0x80000001
#define  EXTENDED_PROCESSOR_BRAND_PT1              0x80000002
#define  EXTENDED_PROCESSOR_BRAND_PT2              0x80000003
#define  EXTENDED_PROCESSOR_BRAND_PT3              0x80000004
#define  EXTENDED_L1_CACHE_TLB                     0x80000005
#define  EXTENDED_L2_CACHE_TLB                     0x80000006
#define  EXTENDED_ADVANCED_POWER_MANAGEMENT_INFO   0x80000007
#define  EXTENDED_TOPOLOGY                         0x80000008

/**
 * Convenient __get_cpuid wrapper macro.
 */
#define get_cpuid(PARAM) __get_cpuid(PARAM, &eax, &ebx, &ecx, &edx)

/**
 * Globals: cpuid support and register values.
 */
int parameters = -1;
int extended   = -1;
unsigned int eax, ebx, ecx, edx;

/**
 * Function declarations.
 */
int detect_support (void);
int highest_function_parameter (void);
int highest_extended_function_parameter (void);
void print_vendor_id (void);
void register_to_chars (unsigned int, char* result, int length);

/**
 * Test code.
 */
int main () {
  if (!detect_support()) return -1;
  if (parameters >= VENDOR_ID) print_vendor_id();
}

/**
 * Library functions.
 */
void print_vendor_id () {
  get_cpuid(VENDOR_ID);
  /**
   * We start out with a 2D array of null-terminators ('\0' or 0). We fill
   * each row with the characters we get back from our call to
   * `get_cpuid(VENDOR_ID)`, and the extra row in the array ensures that
   * our string is null-terminated. (4 times, in fact. Which is enough.)
   */
  char vendor_id[4][4] = { 0 };
  register_to_chars(ebx, vendor_id[0], 4);
  register_to_chars(edx, vendor_id[1], 4);
  register_to_chars(ecx, vendor_id[2], 4);
  printf("vendor id: %s\n", vendor_id[0]);
}

void register_to_chars (unsigned int exx, char* result, int length) {
  assert(length == 4);
  const unsigned int mask = 0xff;
  for (int char_index = 0; char_index < 5; char_index++) {
    result[char_index] = (exx & mask);
    exx >>= 8;
  }
}

int detect_support () {
  highest_function_parameter();
  highest_extended_function_parameter();
  return parameters > -1 ? 1 : 0;
}

int highest_function_parameter () {
  if (!get_cpuid(HIGHEST_FUNCTION_PARAMETER)) {
    fprintf(stderr, "✗ cpuid is not supported by this processor\n");
    return -1;
  } else {
    parameters = eax;
    printf("cpuid: supports leaves 0‒%d\n", parameters);
    return 0;
  }
}

int highest_extended_function_parameter () {
  if (!get_cpuid(EXTENDED_HIGHEST_FUNCTION_PARAMETER)) {
    fprintf(stderr, "⚠ cpuid does not support extended functions\n");
    return -1;
  } else {
    extended = eax;
    printf("cupid: supports extended leaves 0x80000000‒0x%x\n", extended);
    return 0;
  }
}
