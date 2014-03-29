#include "ingredient.h"


/*
typedef enum {
	INGREDIENT_SUCCESS,			 Operation succeeded 						  
	INGREDIENT_NULL_ARGUMENT,	 A NULL argument was passed 				  
	INGREDIENT_BAD_NAME,		 An invalid name was passed				  
	INGREDIENT_BAD_KOSHER_TYPE,	 An invalid kosher type was passed		  
	INGREDIENT_BAD_CALORIES,	 An invalid calories number was passed	  
	INGREDIENT_BAD_HEALTH,		 An invalid health number was passed		  
	INGREDIENT_BAD_COST,		 An invalid cost value was passed			  
	INGREDIENT_BAD_DISCOUNT,	 An invalid discount value was passed		  
	INGREDIENT_SMALL_BUFFER	     The passed buffer is too small			  
} IngredientResult; */

Ingredient ingredientInitialize(const char* name, KosherType kosherType,
		int calories, int health, double cost, IngredientResult* result)	{
		Ingredient newIngredient;
		IF_IS_NULL(name) {
			SET_RESULT(INGREDIENT_NULL_ARGUMENT,result)
			return newIngredient;
		}
		IF_NOT_BETWEEN(strlen(name),0,INGREDIENT_MAX_NAME_LENGTH) {
			SET_RESULT(INGREDIENT_BAD_NAME,result)
			return newIngredient;
		}
		IF_NOT_BETWEEN(kosherType,0,INGREDIENT_KOSHER_TYPE_VALUES-1) {
			SET_RESULT(INGREDIENT_BAD_KOSHER_TYPE,result)
			return newIngredient;
		}
		IF_NOT_BETWEEN(calories,INGREDIENT_MIN_CALORIES,INGREDIENT_MAX_CALORIES) {
			SET_RESULT(INGREDIENT_BAD_CALORIES,result)
			return newIngredient;
		}
		IF_NOT_BETWEEN(health,INGREDIENT_MIN_HEALTH,INGREDIENT_MAX_HEALTH) {
			SET_RESULT(INGREDIENT_BAD_HEALTH,result)
			return newIngredient;
		}
		if (cost <= 0) {
			SET_RESULT(INGREDIENT_BAD_COST,result)
			return newIngredient;
		}
		strcpy(newIngredient.name,name);
		newIngredient.kosherType = kosherType;
		newIngredient.calories = calories;
		newIngredient.health = health;
		newIngredient.cost = cost;
		SET_RESULT(INGREDIENT_SUCCESS,result)
		return newIngredient;
}

IngredientResult ingredientGetName(Ingredient ingredient, char* buffer, int length)	{
	IF_IS_NULL(buffer) {
		return INGREDIENT_NULL_ARGUMENT;
	}
	IF_IS_NULL(ingredient.name) {
		return INGREDIENT_BAD_NAME;
	}
	if (strlen(ingredient.name) > length) {
		return INGREDIENT_SMALL_BUFFER;
	}
	strcpy(buffer,ingredient.name);
	return INGREDIENT_SUCCESS;
}

IngredientResult ingredientChangeCost(Ingredient* ingredient, double cost, int discount)	{
	IF_IS_NULL(ingredient) {
		return INGREDIENT_NULL_ARGUMENT;
	}
	if ((cost <= 0) || (cost*(1-(discount*0.01)) <= 0)) {
		return INGREDIENT_BAD_COST;
	}
	IF_NOT_BETWEEN(discount,0,100) {
		return INGREDIENT_BAD_DISCOUNT;
	}
	if (!DOUBLE_EQUALS(floor(discount),discount)) { // dunno if we can use -lm for math.h, might have to find another way to test for integer
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














