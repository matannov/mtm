#include "dish.h"

/*
typedef enum {
	DISH_SUCCESS,				 Operation succeeded 						  
	DISH_NULL_ARGUMENT,			 A NULL argument was passed 				  
	DISH_INVALID_FLEXIBILITY,	 An invalid flexibility was passed		  
	DISH_IS_FULL,				 Operation aborted because the dish is full 
	DISH_KOSHER_VIOLATION,		 Operation aboured because it violates
							   	   kosher laws								  
	DISH_INGREDIENT_NOT_FOUND,	 The requested ingredient wasn't found	  
	DISH_IS_EMPTY,				 The given dish is empty					  
	DISH_ALREADY_TASTED,		 The dish was already tasted				  
	DISH_NEVER_TASTED,			 The dish was never tasted				  
	DISH_OUT_OF_MEMORY			 A memory error occured					  
} DishResult; */

Dish dishCreate(const char* name, const char* cook, int maxIngredients) {
	IF_IS_NULL(name) {
		return NULL;
	}
	
	IF_IS_NULL(cook) {
		return NULL;
	}
	
	if (maxIngredients < 1) {
		return NULL;
	}
	
	Dish dish = (Dish)malloc(sizeof(*dish));
	IF_IS_NULL(dish) {
		return NULL;
	}
	dish->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
	IF_IS_NULL(dish->name) {
		dishDestroy(dish);
		return NULL;
	}
	strcpy(dish->name,name);
	
	dish->cook = (char*)malloc(sizeof(char)*(strlen(cook)+1));
	IF_IS_NULL(dish->cook) {
		dishDestroy(dish);
		return NULL;
	}
	strcpy(dish->cook,cook);
	dish->maxIngredients = maxIngredients;
	
	dish->ingredients = (Ingredient**)malloc(sizeof(Ingredient*)*maxIngredients);
	IF_IS_NULL(dish->ingredients) {
		dishDestroy(dish);
		return NULL;
	}
	for (int i=0;i<maxIngredients;i++) {
		dish->ingredients[i] = NULL;
	}
	return dish;
}

void dishDestroy(Dish dish) {
	SAFE_FREE(dish->name);
	SAFE_FREE(dish->cook);
	for (int i=0;i<dish->maxIngredients;i++) {
		SAFE_FREE(dish->ingredients[i]);
	}
	SAFE_FREE(dish->ingredients);
	SAFE_FREE(dish);
}

Dish dishClone(Dish source) {
	IF_IS_NULL(source) {
		return NULL;
	}
	IF_IS_NULL(source->name) {
		return NULL;
	}
	IF_IS_NULL(source->cook) {
		return NULL;
	}
	Dish dish = dishCreate(source->name,source->cook,source->maxIngredients);
	Ingredient ingredient;
	Ingredient sourceIngredient;
	IngredientResult result = INGREDIENT_SUCCESS;
	char name[INGREDIENT_MAX_NAME_LENGTH+1];
	for (int i=0;i<source->maxIngredients;i++) {
		if (source->ingredients[i] != NULL) {
			sourceIngredient = *(source->ingredients[i]);
			dish->ingredients[i] = (Ingredient*)malloc(sizeof(Ingredient));
			
			result = ingredientGetName(sourceIngredient,name,INGREDIENT_MAX_NAME_LENGTH);
			if (result != INGREDIENT_SUCCESS) {
				dishDestroy(dish);
				return NULL;
			}
			
			ingredient = ingredientInitialize(name, sourceIngredient.kosherType,
							sourceIngredient.calories, sourceIngredient.health, sourceIngredient.cost, &result);
			if (result != INGREDIENT_SUCCESS) {
				dishDestroy(dish);
				return NULL;
			}
			*(dish->ingredients[i]) = ingredient;  // honestly unsure if part or all of this will disappear once we exit scope
		}
	}
	return dish;
}