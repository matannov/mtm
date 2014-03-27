/*
 * ingredient_test.c
 *
 *  Created on: 9 במרץ 2014
 *      Author: dor
 */

#include "ingredient.h"
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

#define ASSERT_NO_CRASH(expr) expr
#define ASSERT_EQUALS(expr,expected) ASSERT((expr) == (expected))
#define ASSERT_DOUBLE_EQUALS(expr,expected) ASSERT(DOUBLE_EQUALS(expr, expected))

#define ASSERT_SUCCESS(expr) ASSERT_EQUALS(expr, INGREDIENT_SUCCESS)
#define ASSERT_TRUE(expr) ASSERT_EQUALS(expr, true)
#define ASSERT_FALSE(expr) ASSERT_EQUALS(expr, false)
#define ASSERT_NULL_ARGUMENT(expr) ASSERT_EQUALS(expr, INGREDIENT_NULL_ARGUMENT)
#define ASSERT_BAD_CALORIES(expr) ASSERT_EQUALS(expr, INGREDIENT_BAD_CALORIES)
#define ASSERT_BAD_HEALTH(expr) ASSERT_EQUALS(expr, INGREDIENT_BAD_HEALTH)
#define ASSERT_BAD_COST(expr) ASSERT_EQUALS(expr, INGREDIENT_BAD_COST)

static bool testInitialize() {

	ASSERT_NO_CRASH(ingredientInitialize(NULL, MEATY, 10, 2, 14.5, NULL));

	IngredientResult result;

	ingredientInitialize(NULL, PARVE, 30, 10, 1, &result);
	ASSERT_NULL_ARGUMENT(result);

	ingredientInitialize("Tomato", PARVE, 10000, 2, 2, &result);
	ASSERT_BAD_CALORIES(result);

	ingredientInitialize("Tomato", MILKY, 1000, -2, -4, &result);
	ASSERT_BAD_HEALTH(result);

	Ingredient ing = ingredientInitialize("Tomato", PARVE, 30, 10, 1.1, NULL);
	ASSERT_EQUALS(ing.calories, 30);
	ASSERT_DOUBLE_EQUALS(ing.cost, 1.1);

	return true;
}

static bool testGetName() {
	Ingredient ing = ingredientInitialize("Tomato", MEATY, 10, 10, 10, NULL);

	char name[INGREDIENT_MAX_NAME_LENGTH + 1];
	ASSERT_SUCCESS(ingredientGetName(ing, name, INGREDIENT_MAX_NAME_LENGTH + 1));
	ASSERT_EQUALS(0, strcmp(name, "Tomato"));

	ASSERT_NULL_ARGUMENT(ingredientGetName(ing, NULL, 10000));

	return true;
}

static bool testChangeCost() {
	Ingredient ing = ingredientInitialize("Tomato", PARVE, 10, 10, 10, NULL);

	ASSERT_NULL_ARGUMENT(ingredientChangeCost(NULL, 0, 0));
	ASSERT_BAD_COST(ingredientChangeCost(&ing, -4, 4));

	ASSERT_SUCCESS(ingredientChangeCost(&ing, 100, 40));
	ASSERT_DOUBLE_EQUALS(ing.cost, 60);

	return true;
}

static bool testGetQuality() {
	Ingredient ing = ingredientInitialize("A", MEATY, 400, 6, 1, NULL);
	ASSERT_EQUALS(ingredientGetQuality(ing), 4);

	return true;
}

static bool testIsCheaper() {

	Ingredient ing1 = ingredientInitialize("Tomato", PARVE, 10, 10, 10, NULL);
	Ingredient ing2 = ingredientInitialize("Tomato", PARVE, 10, 10, 20, NULL);
	Ingredient ing3 = ingredientInitialize("Tomato", PARVE, 10, 10, 30, NULL);

	ASSERT_TRUE(ingredientIsCheaper(ing1, ing2));
	ASSERT_FALSE(ingredientIsCheaper(ing1, ing1));
	ASSERT_FALSE(ingredientIsCheaper(ing3, ing1));

	return true;
}

static bool testIsBetter() {

	Ingredient ing1 = ingredientInitialize("Tomato", PARVE, 10, 10, 5, NULL);
	Ingredient ing2 = ingredientInitialize("Tomato", PARVE, 20, 8, 10, NULL);
	Ingredient ing3 = ingredientInitialize("Tomato", PARVE, 30, 2, 20, NULL);

	ASSERT_TRUE(ingredientIsBetter(ing1, ing2));
	ASSERT_TRUE(ingredientIsBetter(ing2, ing3));
	ASSERT_FALSE(ingredientIsBetter(ing3, ing1));

	return true;
}

static bool testAreKosher() {

	Ingredient ing1 = ingredientInitialize("Tomato", MILKY, 10, 10, 5, NULL);
	Ingredient ing2 = ingredientInitialize("Tomato", PARVE, 20, 8, 10, NULL);
	Ingredient ing3 = ingredientInitialize("Tomato", MEATY, 20, 8, 10, NULL);

	ASSERT_TRUE(ingredientsAreKosher(ing1, ing2));
	ASSERT_FALSE(ingredientsAreKosher(ing1, ing3));

	return true;
}

int main() {

	RUN_TEST(testInitialize);
	RUN_TEST(testGetName);
	RUN_TEST(testChangeCost);
	RUN_TEST(testGetQuality);
	RUN_TEST(testIsCheaper);
	RUN_TEST(testIsBetter);
	RUN_TEST(testAreKosher);

	return 0;
}
