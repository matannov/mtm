/*
 * ingredient.h
 *
 * Matam Ex. 2 - Spring 2014
 */

#ifndef INGREDIENT_H_
#define INGREDIENT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*******************************************************************************
 * Defines & Enums
 ******************************************************************************/
#define INGREDIENT_MAX_NAME_LENGTH 40

#define INGREDIENT_MIN_CALORIES 0
#define INGREDIENT_MAX_CALORIES 2000

#define INGREDIENT_MIN_HEALTH 0
#define INGREDIENT_MAX_HEALTH 10

#define INGREDIENT_MIN_QUALITY 0
#define INGREDIENT_MAX_QUALITY 10

#define INGREDIENT_KOSHER_TYPE_VALUES 3

#define SET_RESULT(result,ptr) \
		if (ptr != NULL) { \
			*ptr = result; \
		}
		
#define IF_NOT_BETWEEN(val,min,max) \
		if ((min > val) || (val > max))
		
#define IF_IS_NULL(val) \
	if (val == NULL)
	
#define IF_NOT(val) \
	if (!(val))

typedef enum {
	MEATY, MILKY, PARVE
} KosherType;

#define DOUBLE_EQUALS(a,b) (abs((a) - (b)) <= 1e-6)

/*******************************************************************************
 * Ingredient Struct
 ******************************************************************************/
typedef struct ingredient_t {
	char name[INGREDIENT_MAX_NAME_LENGTH + 1];
	KosherType kosherType;
	int calories;
	int health;
	double cost;
} Ingredient;

/*******************************************************************************
 * Return Value Definition
 ******************************************************************************/
typedef enum {
	INGREDIENT_SUCCESS,			/* Operation succeeded 						  */
	INGREDIENT_NULL_ARGUMENT,	/* A NULL argument was passed 				  */
	INGREDIENT_BAD_NAME,		/* An invalid name was passed				  */
	INGREDIENT_BAD_KOSHER_TYPE,	/* An invalid kosher type was passed		  */
	INGREDIENT_BAD_CALORIES,	/* An invalid calories number was passed	  */
	INGREDIENT_BAD_HEALTH,		/* An invalid health number was passed		  */
	INGREDIENT_BAD_COST,		/* An invalid cost value was passed			  */
	INGREDIENT_BAD_DISCOUNT,	/* An invalid discount value was passed		  */
	INGREDIENT_SMALL_BUFFER	    /* The passed buffer is too small			  */
} IngredientResult;

/*******************************************************************************
 * Functions Declarations
 ******************************************************************************/
/*
 * Initialize an ingredient's name, kosher type, calories, health value and cost.
 * If one of the parameters isn't valid, the content of the returned ingredient
 * struct is undefined.
 * Also, keep in mind that the player's name should not be changed if the
 * string passed as name parameter is changed after the function call.
 *
 * Note that some of the parameters have certain limitations on their acceptable
 * values. These limitations can be found in the PDF and if such a limitation
 * doesn't apply, a relevant error code should be placed in @result if not NULL.
 *
 * The Success or error code of the operation will be put in result.
 * But, if the error code is of no interest to the caller, NULL can be passed.
 *
 * @param name: The ingredient's name.
 * @param kosherType: The ingredient's kosherType.
 * @param calories: The ingredient's calories.
 * @param health: The ingredient's health value.
 * @param cost: The ingredient's cost.
 * @param result: The success or error code will be placed here if not NULL.
 * @return The initialized ingredient.
 */
Ingredient ingredientInitialize(const char* name, KosherType kosherType,
		int calories, int health, double cost, IngredientResult* result);

/*
 * Place the ingredient's name in the given buffer.
 *
 * If the given buffer is NULL, INGREDIENT_NULL_ARGUMENT should be returned.
 * If the given buffer length is insufficient, INGREDIENT_SMALL_BUFFER should
 * be returned. This should also be returned if the length is negative.
 *
 * @param ingredient The ingredient to get it's name.
 * @param buffer The buffer to write the name to.
 * @param length The length of the given buffer.
 * @return Success or error code.
 */
IngredientResult ingredientGetName(Ingredient ingredient, char* buffer, int length);

/*
 * Change the ingredient's cost, taking into consideration a certain discount.
 *
 * The ingredient's new cost will be the given cost, minus the discount percentage.
 * The discount is the precentage of the given discount.
 *
 * For example, if cost = 300 and discount = 20 (meaning, 20% off),
 * the new cost will be (1 - 0.2) * 300 = 240.
 *
 * The base cost should satisfy the conditions for a valid ingredient cost,
 * and the cost after the discount should also satisfy those conditions.
 * If such a condition is not met, INGREDIENT_BAD_COST should be returned.
 *
 * The discount should be an integer between 0 and 100 (inclusive).
 * If the discount is invalid, INGREDIENT_BAD_DISCOUNT should be returned.
 *
 * @param ingredient A pointer to the ingredient that needs updating
 * @param cost The ingredient's new base cost
 * @param discount The discount in percentage
 * @return Success or error code
 */
IngredientResult ingredientChangeCost(Ingredient* ingredient, double cost, int discount);

/*
 * Returns the quality of the ingredient.
 * The quality computation formula is given in the PDF.
 *
 * For the normalization of the health and the calories, you may assume the following:
 * 	- INGREDIENT_MIN_CALORIES == 0
 * 	- INGREDIENT_MIN_HEALTH == 0
 * 	- INGREDIENT_MIN_QUALITY == 0
 * You may **not** assume anything about INGREDIENT_MAX_{CALORIES, HEALTH, QUALITY}.
 * This means the normalization must be correct even if we change those defines.
 *
 * @param ingredient The ingredient to get the quality of.
 * @return The ingredient's quality.
 */
double ingredientGetQuality(Ingredient ingredient);

/*
 * Returns true if ingredient1 is cheaper than ingredient2, and false otherwise.
 *
 * Note: If the costs are equal, the function should return false.
 *
 * @param ingredient1 The first ingredient
 * @param ingredient2 The second ingredient
 * @return Whether the first ingredient is cheaper than the second
 */
bool ingredientIsCheaper(Ingredient ingredient1, Ingredient ingredient2);

/*
 * Returns true if ingredient1 is better than ingredient2.
 *
 * Ingredient1 is said to be better than ingredient2 if it's healthier, has
 * less calories and is cheaper.
 * Note: all the inequalities in the definition are *strong* inequalities.
 * Note: An ingredient is "healthier" if it has a bigger health value.
 *
 * @param ingredient1 The first ingredient
 * @param ingredient2 The second ingredient
 * @return Whether the first ingredient is better than the second
 */
bool ingredientIsBetter(Ingredient ingredient1, Ingredient ingredient2);

/*
 * Returns true if the ingredients can be places together in the same dish,
 * in terms of Kosher-ness.
 *
 * We'll say that the ingredients violate the kosher laws if one of them
 * is Meaty and the other one is Milky (neverminding the order).
 * In any other case, they may be places together.
 *
 * @param ingredient1 The first ingredient.
 * @param ingredient2 The second ingredient.
 * @return Whether the ingredients can be places together or not.
 */
bool ingredientsAreKosher(Ingredient ingredient1, Ingredient ingredient2);

#endif /* INGREDIENT_H_ */














