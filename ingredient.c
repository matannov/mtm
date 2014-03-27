#include <stdio.h>
#include <string.h>
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
		Ingredient * newIngredient = (Ingredient *)malloc(sizeof(Ingredient));
		if (name == NULL) {
			SET_RESULT(INGREDIENT_NULL_ARGUMENT,result)
			return *newIngredient;
		}
		if (strlen(name) > INGREDIENT_MAX_NAME_LENGTH) {
			SET_RESULT(INGREDIENT_BAD_NAME,result)
			return *newIngredient;
		}
		IF_NOT_BETWEEN(kosherType,0,INGREDIENT_KOSHER_TYPE_VALUES-1) {
			SET_RESULT(INGREDIENT_BAD_KOSHER_TYPE,result)
			return *newIngredient;
		}
		IF_NOT_BETWEEN(calories,INGREDIENT_MIN_CALORIES,INGREDIENT_MAX_CALORIES) {
			SET_RESULT(INGREDIENT_BAD_CALORIES,result)
			return *newIngredient;
		}
		IF_NOT_BETWEEN(health,INGREDIENT_MIN_HEALTH,INGREDIENT_MAX_HEALTH) {
			SET_RESULT(INGREDIENT_BAD_HEALTH,result)
			return *newIngredient;
		}
		if (cost <= 0) {
			SET_RESULT(INGREDIENT_BAD_COST,result)
			return *newIngredient;
		}
		strcpy(newIngredient->name,name);
		newIngredient->kosherType = kosherType;
		newIngredient->calories = calories;
		newIngredient->health = health;
		newIngredient->cost = cost;
		SET_RESULT(INGREDIENT_SUCCESS,result)
		return *newIngredient;
}

IngredientResult ingredientGetName(Ingredient ingredient, char* buffer, int length)	{
	return INGREDIENT_SUCCESS;
}

IngredientResult ingredientChangeCost(Ingredient* ingredient, double cost, int discount)	{
	return INGREDIENT_SUCCESS;
}

double ingredientGetQuality(Ingredient ingredient)	{
	return 0;
}

bool ingredientIsCheaper(Ingredient ingredient1, Ingredient ingredient2)	{
	return true;
}

bool ingredientIsBetter(Ingredient ingredient1, Ingredient ingredient2)	{
	return true;
}

bool ingredientsAreKosher(Ingredient ingredient1, Ingredient ingredient2)	{
	return true;
}














