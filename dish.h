/*
 * dish.h
 *
 *  Created on: 10 במרץ 2014
 *      Author: dor
 */

#ifndef DISH_H_
#define DISH_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "ingredient.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define SAFE_FREE(val) \
	if (val != NULL) { \
		free(val); \
	}

#define CHECK_NULL_ARG(val) \
	if (val == NULL) { \
		return DISH_NULL_ARGUMENT; \
	}

/*******************************************************************************
 * Dish Struct
 ******************************************************************************/
typedef struct dish_t {
	char * name;
	char * cook;
	Ingredient ** ingredients;
	int maxIngredients;
	int currentIngredients;
	int tasted;
	int liked;
}* Dish;

/*******************************************************************************
 * Return Value Definition
 ******************************************************************************/
typedef enum {
	DISH_SUCCESS,				/* Operation succeeded 						  */
	DISH_NULL_ARGUMENT,			/* A NULL argument was passed 				  */
	DISH_INVALID_FLEXIBILITY,	/* An invalid flexibility was passed		  */
	DISH_IS_FULL,				/* Operation aborted because the dish is full */
	DISH_KOSHER_VIOLATION,		/* Operation aboured because it violates
							   	   kosher laws								  */
	DISH_INGREDIENT_NOT_FOUND,	/* The requested ingredient wasn't found	  */
	DISH_IS_EMPTY,				/* The given dish is empty					  */
	DISH_ALREADY_TASTED,		/* The dish was already tasted				  */
	DISH_NEVER_TASTED,			/* The dish was never tasted				  */
	DISH_OUT_OF_MEMORY			/* A memory error occured					  */
} DishResult;

/*
 * These return values apply for all functions:
 * If the operation succeeds: DISH_SUCCESS
 * NULL parameter was passed: DISH_NULL_ARGUMENT
 * Memory allocation error occured: DISH_OUT_OF_MEMORY
 * Dish has no room for more ingredients: DISH_IS_FULL
 * Dish has no ingredients: DISH_IS_EMPTY
 * The passed ingredient index is negative or out of bounds: DISH_INGREDIENT_NOT_FOUND
 * An attempt to add an ingredients that violets kosher laws: DISH_KOSHER_VIOLATION
 */

/*******************************************************************************
 * Functions Declarations
 ******************************************************************************/
/*
 * Create a new empty dish.
 * The function accepts the maximal number of ingredients the new dish may hold.
 * If this number is 0 or negative, NULL should be returned.
 * The function also accepts the name of the dish itself and the name of the cook.
 *
 * The function should make sure that after passing a name and initializing a dish,
 * changing that name after the initialization does not change the name of the dish.
 * The same applies for the cook's name.
 *
 * The function creates a new dish, hence, the created dish was never tasted nor
 * liked by any judge.
 *
 * @param name The dish's name
 * @param cook The cook's name
 * @param maxIngredients The maximal number of ingredients the dish may hold.
 * @return The newly created dish, or NULL if any error occured.
 */
Dish dishCreate(const char* name, const char* cook, int maxIngredients);

/*
 * Destroy a given dish, deallocating all necessary memory.
 *
 * @param dish The dish to destroy.
 */
void dishDestroy(Dish dish);

/*
 * Clones a dish, creating a new dish that's an exact clone of the source dish.
 *
 * The new dish will have the same name, same cook and same ingredients as the
 * source dish. It will also be able to hold the same maximal number of ingredients.
 * The new dish will also have the same ingredients as the source dish.
 *
 * The new dish is, as it's name suggests, new. Hence, no judge ever got a chance
 * to taste it.
 *
 * @param source The source dish.
 * @return The new dish, or NULL in any case of error.
 */
Dish dishClone(Dish source);

/*
 * Add an ingredient to a dish.
 *
 * In order for the function to succeed, three conditions must be met:
 * 	1. The dish must have enough room for the new ingredient.
 * 	2. The ingredient must not violate kosher laws with any other ingredient
 * 	   currently in the dish.
 * 	   The kosher laws are defined in ingredient.h.
 * 	3. The dish was never tasted before.
 * In case one of these terms doesn't apply, an error code should be returned.
 *
 * @param dish The dish to add to.
 * @param ingredient The ingredient to add.
 * @return Success or error code.
 */
DishResult dishAddIngredient(Dish dish, Ingredient ingredient);

/*
 * Remove an ingredient from a dish, using the ingredient's index.
 *
 * There are a few important notes about the index, please read them in the PDF.
 *
 * If the dish was already tasted, an ingredient may not be removed.
 * In this case an error code should be returned.
 *
 * @param dish The dish to remove from.
 * @param index The ingredient's index.
 * @return Success or error code.
 */
DishResult dishRemoveIngredient(Dish dish, int index);

/*
 * Returns the dish's name.
 * The function should allocate a buffer for the dish's name and return a
 * pointer to the buffer via @name.
 *
 * @param dish The dish to get the name of.
 * @param name The pointer to the newly allocated name should be placed here.
 * @return Success or error code.
 */
DishResult dishGetName(Dish dish, char** name);

/*
 * Returns the dish's cook.
 * The function should allocate a buffer for the dish's cook and return a
 * pointer to the buffer via @cook.
 *
 * @param dish The dish to get the cook of.
 * @param cook The pointer to the newly allocated cook should be placed here.
 * @return Success or error code.
 */
DishResult dishGetCook(Dish dish, char** cook);

/*
 * Sets the dish's name.
 * Note that changing the passed name after the function ends does not change
 * the dish's name.
 *
 * @param dish The dish to set the name of.
 * @param name The new name.
 * @return Success or error code.
 */
DishResult dishSetName(Dish dish, const char* name);

/*
 * Test if there are two ingredients in a dish that have the same name.
 *
 * If the dish is empty, DISH_IS_EMPTY should be returned, and @areDuplicate
 * will contain the value @{false}.
 *
 * @param dish The dish to test.
 * @param areDuplicate The result should be placed here.
 * @return Success or error code.
 */
DishResult dishAreDuplicateIngredients(Dish dish, bool* areDuplicate);

/*
 * This function is called after a judge tastes a dish, and updates the dish
 * accordingly.
 *
 * @param dish The dish that was tasted.
 * @param liked Holds whether the judge liked the dish or not.
 * @return Success or error code
 */
DishResult dishTaste(Dish dish, bool liked);

/*
 * Returns how much a dish is tasty.
 * We say that a dish's "tastiness" is the ratio between the amount of times
 * the dish was liked and the amount of times the dish was tasted.
 *
 * If a dish was never tasted, a relevant error code should be returned.
 *
 * @param dish The dish to test.
 * @param tastiness The dish's tastiness will be placed here.
 * @return Success or error code.
 */
DishResult dishHowMuchTasty(Dish dish, double* tastiness);

/*
 * Returns the dish's quality.
 * A dish's quality is defined as the average quality of all it's ingredients.
 * If the dish is empty, a relevant error code should be returned.
 *
 * @param dish The dish to test the quality of.
 * @param quality The dish's quality should be placed here.
 * @return Success or error code.
 */
DishResult dishGetQuality(Dish dish, double* quality);

/*
 * The function returns whether dish1 is better than dish2.
 * We'll say that dish1 is better than dish2 if:
 * 	1. It has better quality than dish2 (not equal).
 * 	2. dish1's price <= dish2's price + flexibility%
 *
 * A dish's price is simply the sum of all of it's ingredient's prices.
 *
 * The flexibility is given in percents, and it tells how much percent the user
 * is willing to pay extra, in regard to dish2's price.
 *
 * If the given flexibility is not between 0 and 1 (inclusive),
 * an error code should be returned.
 * For example, to specify 40% flexibility, 0.4 should be passed.
 *
 * @param dish1 The first dish.
 * @param dish2 The second dish.
 * @param flexibility The price flexibility.
 * @param isBetter The result should be placed here.
 * @return Success or error code.
 */
DishResult dishIsBetter(Dish dish1, Dish dish2, double flexibility, bool* isBetter);

#endif /* DISH_H_ */
