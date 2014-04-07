#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "ingredient.h"

//IN_RANGE macro is not limited to a certain type
#define IN_RANGE(value, min, max) \
	(((value) >= (min) && (value) <= (max)) ? true : false)
	
#define NULL_ARG(val) \
	if (val == NULL) {	return INGREDIENT_NULL_ARGUMENT;	}

/*******************************************************************************
 * static internal functions
 ******************************************************************************/
static bool isValidName(const char* name) {
	int const INGREDIENT_MIN_NAME_LENGTH = 1;
	return IN_RANGE(strlen(name), INGREDIENT_MIN_NAME_LENGTH, 
		INGREDIENT_MAX_NAME_LENGTH);
}

static bool isValidKosherType(KosherType kosherType) {
	int const KOSHER_FIRST_VALUE = 0;
	return IN_RANGE(kosherType, KOSHER_FIRST_VALUE, 
		INGREDIENT_KOSHER_TYPE_VALUES-1);
}

static bool isValidCalories(int calories) {
	return IN_RANGE(calories, INGREDIENT_MIN_CALORIES,
		INGREDIENT_MAX_CALORIES);
}

static bool isValidHealth(int health) {
	return IN_RANGE(health, INGREDIENT_MIN_HEALTH, INGREDIENT_MAX_HEALTH);
}

static bool isValidCost(double cost) {
	double const INGREDIENT_MIN_COST = 0.0;
	return (isfinite(cost) && cost >= INGREDIENT_MIN_COST);
}

static IngredientResult checkInputForInitialize(const char* name, KosherType 
	kosherType, int calories, int health, double cost) {

	NULL_ARG(name)

	if(!isValidName(name)) {
		return INGREDIENT_BAD_NAME;
	}
	if(!isValidKosherType(kosherType)) {
		return INGREDIENT_BAD_KOSHER_TYPE;
	}
	if(!isValidCalories(calories)) {
		return INGREDIENT_BAD_CALORIES;
	}
	if(!isValidHealth(health)) {
		return INGREDIENT_BAD_HEALTH;
	}
	if(!isValidCost(cost)) {
		return INGREDIENT_BAD_COST;
	}
	return INGREDIENT_SUCCESS;
}

/*******************************************************************************
 * interface functions
 ******************************************************************************/

Ingredient ingredientInitialize(const char* name, KosherType kosherType,
	int calories, int health, double cost, IngredientResult* result) {

	Ingredient newIngredient;
	IngredientResult tempResult;
	tempResult = checkInputForInitialize(name, kosherType, 
		calories, health, cost);
	if(result != NULL) {
		*result = tempResult;
	}
	if(tempResult != INGREDIENT_SUCCESS) {
		return newIngredient;
	}
	
	strcpy(newIngredient.name, name);
	newIngredient.kosherType = kosherType;
	newIngredient.calories = calories;
	newIngredient.health = health;
	newIngredient.cost = cost;

	return newIngredient;
}

IngredientResult ingredientGetName(Ingredient ingredient, char* buffer, 
	int length) {
	NULL_ARG(buffer)
	
	if(length < 0 || strlen(ingredient.name) + 1 > length) {
		return INGREDIENT_SMALL_BUFFER;
	}

	strcpy(buffer,ingredient.name);
	return INGREDIENT_SUCCESS;
}

IngredientResult ingredientChangeCost(Ingredient* ingredient, double cost, int discount)	{
	NULL_ARG(ingredient)
	
	if (!isValidCost(cost)) {
		return INGREDIENT_BAD_COST;
	}
	IF_NOT_BETWEEN(discount,0,100) {
		return INGREDIENT_BAD_DISCOUNT;
	}
	ingredient->cost = cost*(1-(discount*0.01));
	return INGREDIENT_SUCCESS;
}

double ingredientGetQuality(Ingredient ingredient)	{
	double calories = 0;
	double health = 0;
	if ((INGREDIENT_MAX_CALORIES-INGREDIENT_MIN_CALORIES) == 0) {
		return INGREDIENT_BAD_CALORIES;
	}
	if ((INGREDIENT_MAX_HEALTH-INGREDIENT_MIN_HEALTH) == 0) {
		return INGREDIENT_BAD_HEALTH;
	}
	calories = ingredient.calories*(10.0/(INGREDIENT_MAX_CALORIES-INGREDIENT_MIN_CALORIES));
	health = ingredient.health*(10.0/(INGREDIENT_MAX_HEALTH-INGREDIENT_MIN_HEALTH));
	if (health-calories > 0) {
		return health-calories;
	}
	return 0;
}

bool ingredientIsCheaper(Ingredient ingredient1, Ingredient ingredient2)	{
	if (ingredient1.cost < ingredient2.cost) {
		return true;
	}
	return false;
}

static bool ingredientHasLessCalories(Ingredient ingredient1, Ingredient ingredient2) {
	if (ingredient1.calories < ingredient2.calories) {
		return true;
	}
	return false;
}

static bool ingredientIsHealthier(Ingredient ingredient1, Ingredient ingredient2) {
	if (ingredient1.health > ingredient2.health) {
		return true;
	}
	return false;
}

bool ingredientIsBetter(Ingredient ingredient1, Ingredient ingredient2)	{
	IF_NOT(ingredientIsCheaper(ingredient1,ingredient2)) {
		return false;
	}
	IF_NOT(ingredientHasLessCalories(ingredient1,ingredient2)) {
		return false;
	}
	IF_NOT(ingredientIsHealthier(ingredient1,ingredient2)) {
		return false;
	}
	return true;
}

bool ingredientsAreKosher(Ingredient ingredient1, Ingredient ingredient2)	{
	if (ingredient1.kosherType == MILKY && ingredient2.kosherType == MEATY) {
		return false;
	}
	if (ingredient1.kosherType == MEATY && ingredient2.kosherType == MILKY) {
		return false;
	}
	return true;
}














