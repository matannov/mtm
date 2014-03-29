#include "dish.h"
#include <stdio.h>
#include <string.h>

#define ASSERT(expr) do { \
	if(!(expr)) { \
		printf("\nAssertion failed %s (%s:%d).\n", #expr, __FILE__, __LINE__); \
		return false; \
	} else { \
		printf("."); \
	} \
} while (0)

#define RUN_TEST(test) do { \
  printf("Running "#test); \
  if(test()) { \
    printf("[OK]\n"); \
  } \
} while(0)

#define ASSERT_NO_CRASH(expr) expr; ASSERT(true)
#define ASSERT_EQUALS(expr,expected) ASSERT((expr) == (expected))
#define ASSERT_NOT_EQUALS(expr,unexpected) ASSERT((expr) != (unexpected))
#define ASSERT_DOUBLE_EQUALS(expr,expected) ASSERT(DOUBLE_EQUALS(expr, expected))
#define ASSERT_STRING_EQUALS(s1,s2) ASSERT(strcmp(s1, s2) == 0)

#define ASSERT_SUCCESS(expr) ASSERT_EQUALS(expr, DISH_SUCCESS)
#define ASSERT_TRUE(expr) ASSERT_EQUALS(expr, true)
#define ASSERT_FALSE(expr) ASSERT_EQUALS(expr, false)
#define ASSERT_NULL(expr) ASSERT_EQUALS(expr, NULL)
#define ASSERT_NOT_NULL(expr) ASSERT_NOT_EQUALS(expr, NULL)
#define ASSERT_NULL_ARGUMENT(expr) ASSERT_EQUALS(expr, DISH_NULL_ARGUMENT)
#define ASSERT_KOSHER_VIOLATION(expr) ASSERT_EQUALS(expr, DISH_KOSHER_VIOLATION)
#define ASSERT_FULL(expr) ASSERT_EQUALS(expr, DISH_IS_FULL)
#define ASSERT_INGREDIENT_NOT_FOUND(expr) ASSERT_EQUALS(expr, DISH_INGREDIENT_NOT_FOUND)
#define ASSERT_EMPTY(expr) ASSERT_EQUALS(expr, DISH_IS_EMPTY)

//TODO: add more tests
static bool testCreate() {
	ASSERT_NULL(dishCreate(NULL,NULL,0));
	ASSERT_NULL(dishCreate(NULL,NULL,1));
	ASSERT_NULL(dishCreate("string",NULL,0));
	ASSERT_NULL(dishCreate("",NULL,0));
	ASSERT_NULL(dishCreate("","",0));
	Dish dish = dishCreate("","",1);
	ASSERT_NOT_NULL(dish);
	dishDestroy(dish);
	
	ASSERT_NULL(dishCreate(NULL, "Dor", 4));
	ASSERT_NULL(dishCreate("Soup", "Shlomo", -1));

	char name[] = "Soup";
	Dish soup = dishCreate(name, "Shlomo", 5);
	ASSERT_NOT_NULL(soup);

	name[0] = 'R';
	dishDestroy(soup);
	char * name1 = (char*)malloc(sizeof(char)*4);
	strcpy(name1,"ttl");
	char * name2 = (char*)malloc(sizeof(char)*4);
	strcpy(name2,"ttl");
	free(name1);
	//ASSERT_NULL(dishCreate(name1,name2,2)); no idea if this can be made not to crash
	free(name2);
	//ASSERT_NULL(dishCreate(name1,name2,2));
	
	return true;
}

//TODO: add more tests
static bool testDestroy() {

	ASSERT_NO_CRASH(dishDestroy(dishCreate("Soup", "Dor", 4)));
	char * name1 = (char*)malloc(sizeof(char)*4);
	strcpy(name1,"ttl");
	char * name2 = (char*)malloc(sizeof(char)*4);
	strcpy(name2,"ttl");
	Dish food = dishCreate(name1,name2,2);
	free(name1);
	free(name2);
	ASSERT_NO_CRASH(dishDestroy(food)); //actually not sure what should happen in this case, but I'm pretty sure it shouldn't crash
	return true;
}
/* move this as more functions are implemented
//TODO: add more tests 
static bool testClone() {

	Dish src = dishCreate("Micky on a Stick", "Micky", 3);

	Dish cpy = dishClone(src);
	ASSERT_NOT_NULL(cpy);

	char* name;
	ASSERT_SUCCESS(dishGetName(cpy, &name));
	ASSERT_STRING_EQUALS(name, "Micky on a Stick");

	free(name);
	dishDestroy(cpy);
	dishDestroy(src);
	return true;
}

//TODO: add more tests
static bool testAddIngredient() {

	Dish dish = dishCreate("Shanim Hasumot", "Shanim Bli Regesh", 2);
	Ingredient ing1 = ingredientInitialize("Shanim Mehatsad", MEATY, 1, 1, 1, NULL);
	Ingredient ing2 = ingredientInitialize("Menutak", MILKY, 1, 1, 1, NULL);
	Ingredient ing3 = ingredientInitialize("Male Be-Uvdot", PARVE, 1, 1, 1, NULL);

	ASSERT_NULL_ARGUMENT(dishAddIngredient(NULL, ing1));

	ASSERT_SUCCESS(dishAddIngredient(dish, ing1));
	ASSERT_KOSHER_VIOLATION(dishAddIngredient(dish, ing2));
	ASSERT_SUCCESS(dishAddIngredient(dish, ing1));
	ASSERT_FULL(dishAddIngredient(dish, ing3));

	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testRemoveIngredient() {

	ASSERT_NULL_ARGUMENT(dishRemoveIngredient(NULL, -1));

	Dish dish = dishCreate("Hasrot Kol Ereh", "Ha Keisar Shel Yapan", 2);
	Ingredient ing1 = ingredientInitialize("Ha Keisar Shel Yapan", MEATY, 1, 1, 1, NULL);

	ASSERT_INGREDIENT_NOT_FOUND(dishRemoveIngredient(dish, 4));
	ASSERT_INGREDIENT_NOT_FOUND(dishRemoveIngredient(dish, -2));

	dishAddIngredient(dish, ing1);
	dishAddIngredient(dish, ing1);

	ASSERT_SUCCESS(dishRemoveIngredient(dish, 0));
	ASSERT_SUCCESS(dishRemoveIngredient(dish, 0));
	ASSERT_INGREDIENT_NOT_FOUND(dishRemoveIngredient(dish, 0));

	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testGetName() {

	Dish dish = dishCreate("Boiled Student", "Stav Doolman", 2);

	char* name;
	ASSERT_SUCCESS(dishGetName(dish, &name));
	ASSERT_STRING_EQUALS(name, "Boiled Student");
	name[0] = 'B';

	char* dupName;
	ASSERT_SUCCESS(dishGetName(dish, &dupName));
	ASSERT_STRING_EQUALS(dupName, "Boiled Student");

	free(name);
	free(dupName);
	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testGetCook() {

	Dish dish = dishCreate("Rotten Shlomo", "Sarai Duek", 2);

	char* cook;
	ASSERT_SUCCESS(dishGetCook(dish, &cook));
	ASSERT_STRING_EQUALS(cook, "Sarai Duek");

	free(cook);
	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testSetName() {

	Dish dish = dishCreate("Sweet & Sour Dor", "Ofer Givoli", 2);

	ASSERT_SUCCESS(dishSetName(dish, "Just Dor"));
	char* name;
	dishGetName(dish, &name);
	ASSERT_STRING_EQUALS(name, "Just Dor");
	free(name);

	char newName[] = "New Blabla";
	ASSERT_SUCCESS(dishSetName(dish, newName));
	newName[0] = 'M';
	dishGetName(dish, &name);
	ASSERT_STRING_EQUALS(name, "New Blabla");
	free(name);

	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testAreDuplicateIngredients() {

	Ingredient ing1 = ingredientInitialize("99 little bugs in the code", MEATY, 1, 1, 1, NULL);
	Ingredient ing2 = ingredientInitialize("99 little bugs in the code", MEATY, 1, 1, 1, NULL);
	Ingredient ing3 = ingredientInitialize("Fix one, Patch it up", MEATY, 1, 1, 1, NULL);
	Ingredient ing4 = ingredientInitialize("127 little bugs in the code", MEATY, 1, 1, 1, NULL);

	Dish dish1 = dishCreate("Buggy Code", "Myself", 5);
	Dish dish2 = dishCreate("Laggy Code", "You", 5);
	bool isDuplicate;

	dishAddIngredient(dish1, ing1);
	dishAddIngredient(dish1, ing2);
	ASSERT_SUCCESS(dishAreDuplicateIngredients(dish1, &isDuplicate));
	ASSERT_TRUE(isDuplicate);

	dishAddIngredient(dish2, ing3);
	dishAddIngredient(dish2, ing4);
	ASSERT_SUCCESS(dishAreDuplicateIngredients(dish2, &isDuplicate));
	ASSERT_FALSE(isDuplicate);

	dishDestroy(dish1);
	dishDestroy(dish2);
	return true;
}

//TODO: add more tests
static bool testTaste() {

	ASSERT_NULL_ARGUMENT(dishTaste(NULL, false));

	return true;
}

//TODO: add more tests
static bool testHowMuchTasty() {

	Dish dish = dishCreate("Tasty", "Me of course", 1);

	ASSERT_SUCCESS(dishTaste(dish, true));
	ASSERT_SUCCESS(dishTaste(dish, true));
	ASSERT_SUCCESS(dishTaste(dish, false));

	double tastiness;
	ASSERT_SUCCESS(dishHowMuchTasty(dish, &tastiness));
	ASSERT_DOUBLE_EQUALS(tastiness, (double)2 / 3);

	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testGetQuality() {

	Dish dish = dishCreate("En Yoter Pahad", "En Yoter Klum", 2);
	Ingredient ing1 = ingredientInitialize("Ani Ish Kash", PARVE, 2000, 1, 1, NULL);
	Ingredient ing2 = ingredientInitialize("Ani Etz Akum", PARVE, 0, 10, 1, NULL);

	dishAddIngredient(dish, ing1);
	dishAddIngredient(dish, ing2);

	double quality;
	ASSERT_SUCCESS(dishGetQuality(dish, &quality));
	ASSERT_DOUBLE_EQUALS(quality, 5);

	dishDestroy(dish);
	return true;
}

//TODO: add more tests
static bool testIsBetter() {

	Dish dish1 = dishCreate("Reva Shaa", "Mehake Barehov", 2);
	Dish dish2 = dishCreate("Lahaver Harusi", "Hadash", 2);

	Ingredient ing1 = ingredientInitialize("Dor", MEATY, 0, 4, 20, NULL);
	Ingredient ing2 = ingredientInitialize("Cohen", MEATY, 0, 6, 30, NULL);

	dishAddIngredient(dish1, ing1);
	dishAddIngredient(dish2, ing2);

	bool isBetter;
	ASSERT_SUCCESS(dishIsBetter(dish2, dish1, 0.7, &isBetter));
	ASSERT_TRUE(isBetter);

	dishDestroy(dish1);
	dishDestroy(dish2);
	return true;
}
*/
int main() {

	/* TODO: read
	 * A challenge to the one with good taste:
	 * 	Which songs can you recognize in the test?
	 * 	Hint: I chose the big hits so it won't be very hard...
	 * 	Email mtm@tx with the subject "songs", and state the test function
	 * 	and the recognized song.
	 * 	Those who will recognize all songs will get a suprise!
	 * 		Unfortunately, the suprise will not include bonus points... :/
	 */
	RUN_TEST(testCreate);
	RUN_TEST(testDestroy);/*
	RUN_TEST(testClone);
	RUN_TEST(testAddIngredient);
	RUN_TEST(testRemoveIngredient);
	RUN_TEST(testGetName);
	RUN_TEST(testGetCook);
	RUN_TEST(testSetName);
	RUN_TEST(testAreDuplicateIngredients);
	RUN_TEST(testTaste);
	RUN_TEST(testHowMuchTasty);
	RUN_TEST(testGetQuality);
	RUN_TEST(testIsBetter);*/

	return 0;
}
