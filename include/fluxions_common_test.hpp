#ifndef FLUXIONS_COMMON_TEST_HPP
#define FLUXIONS_COMMON_TEST_HPP

#define FLUXIONS_TEST(test) fprintf(stderr, "%s(): Test " #test " was %s\n", __FUNCTION__, ((test(#test)) ? "successful" : "unsuccessful"));
#define FLUXIONS_BEGIN_TESTS() bool result = true;
#define FLUXIONS_END_TESTS()   return result;
#define FLUXIONS_DO_TEST(test) if (!(test)) { \
                                  fprintf(stderr, "false <= %s\n", #test); \
                                  result = false; \
                               } else { \
                                  fprintf(stderr, "true  <= %s\n", #test); \
                               }


#endif