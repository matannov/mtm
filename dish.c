#include "dish.h"

static Ingredient ingredientClone(Ingredient ingredient) {
	return ingredientInitialize(ingredient.name, ingredient.kosherType,
		ingredient.calories, ingredient.health, ingredient.cost, NULL);
}

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
	dish->currentIngredients = 0;
	dish->tasted = 0;
	dish->liked = 0;
	
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
			*(dish->ingredients[i]) = ingredientClone(ingredient);
		}
	}
	return dish;
}

DishResult dishAddIngredient(Dish dish, Ingredient ingredient) {
	NULL_ARG(dish)
	if (dish->currentIngredients == dish->maxIngredients) {
		return DISH_IS_FULL;
	}
	for (int i = 0;i < dish->currentIngredients;i++) {
		if (ingredientsAreKosher(ingredient,*(dish->ingredients[i])) == false) {
			return DISH_KOSHER_VIOLATION;
		}
	}
	if (dish->tasted != 0) {
		return DISH_ALREADY_TASTED;
	}
	dish->ingredients[dish->currentIngredients] = (Ingredient*)malloc(sizeof(Ingredient));
	*(dish->ingredients[dish->currentIngredients]) = ingredientClone(ingredient);
	dish->currentIngredients++;
	return DISH_SUCCESS;
}

DishResult dishRemoveIngredient(Dish dish, int index) {
	NULL_ARG(dish)
	IF_NOT_BETWEEN(index,0,dish->maxIngredients-1) {
		return DISH_INGREDIENT_NOT_FOUND;
	}
	IF_IS_NULL(dish->ingredients[index]) {
		return DISH_INGREDIENT_NOT_FOUND;
	}
	if (dish->tasted != 0) {
		return DISH_ALREADY_TASTED;
	}
	free(dish->ingredients[index]);
	for (int i=index+1;i<dish->currentIngredients;i++) {
		dish->ingredients[i-1] = dish->ingredients[i];
	}
	dish->ingredients[dish->currentIngredients-1] = NULL;
	dish->currentIngredients--;
	return DISH_SUCCESS;
}

DishResult dishGetName(Dish dish, char** name) {
	NULL_ARG(dish)
	NULL_ARG(name)
	char * buffer = (char*)malloc(sizeof(char)*(strlen(dish->name)+1));
	IF_IS_NULL(buffer) {
		return DISH_OUT_OF_MEMORY;
	}
	strcpy(buffer,dish->name);
	*name = buffer;
	return DISH_SUCCESS;
}

DishResult dishGetCook(Dish dish, char** cook) {
	NULL_ARG(dish)
	NULL_ARG(cook)
	char * buffer = (char*)malloc(sizeof(char)*(strlen(dish->cook)+1));
	IF_IS_NULL(buffer) {
		return DISH_OUT_OF_MEMORY;
	}
	strcpy(buffer,dish->cook);
	*cook = buffer;
	return DISH_SUCCESS;
}

DishResult dishSetName(Dish dish, const char* name) {
	NULL_ARG(dish)
	NULL_ARG(name)
	char * newName = (char*)malloc(sizeof(char)*(strlen(name)+1));
	IF_IS_NULL(newName) {
		return DISH_OUT_OF_MEMORY;
	}
	strcpy(newName,name);
	free(dish->name);
	dish->name = newName;
	return DISH_SUCCESS;
}

DishResult dishAreDuplicateIngredients(Dish dish, bool* areDuplicate) {
	NULL_ARG(dish)
	NULL_ARG(areDuplicate)
	*areDuplicate = false;
	if (dish->currentIngredients == 0) {
		return DISH_IS_EMPTY;
	}
	for (int i = 0;i<dish->currentIngredients;i++) {
		for (int j = i+1;j<dish->currentIngredients;j++) {
			if (strcmp(dish->ingredients[i]->name,dish->ingredients[j]->name) == 0) {
				*areDuplicate = true;
			}
		}
	}
	return DISH_SUCCESS;
}

DishResult dishTaste(Dish dish, bool liked) {
	NULL_ARG(dish)
	dish->tasted++;
	if (liked == true) {
		dish->liked++;
	}
	return DISH_SUCCESS;
}

DishResult dishHowMuchTasty(Dish dish, double* tastiness) {
	NULL_ARG(dish)
	NULL_ARG(tastiness)
	if (dish->tasted == 0) {
		return DISH_NEVER_TASTED;
	}
	*tastiness = (double)(dish->liked);
	*tastiness /= dish->tasted;
	return DISH_SUCCESS;
}

DishResult dishGetQuality(Dish dish, double* quality) {
	NULL_ARG(dish)
	NULL_ARG(quality);
	if (dish->currentIngredients == 0) {
		return DISH_IS_EMPTY;
	}
	*quality = 0;
	for (int i=0;i < dish->currentIngredients;i++) {
		*quality += ingredientGetQuality(*(dish->ingredients[i]));
	}
	*quality /= dish->currentIngredients;
	return DISH_SUCCESS;
}

DishResult dishIsBetter(Dish dish1, Dish dish2, double flexibility, bool* isBetter) {
	NULL_ARG(dish1)
	NULL_ARG(dish2)
	NULL_ARG(isBetter)
	IF_NOT_BETWEEN(flexibility,0,1) {
		return DISH_INVALID_FLEXIBILITY;
	}
	*isBetter = true;
	double quality1 = 0, quality2 = 0;
	double cost1 = 0, cost2 = 0;
	if (dishGetQuality(dish1,&quality1) != DISH_SUCCESS) {
		return DISH_IS_EMPTY;
	}
	if (dishGetQuality(dish2,&quality2) != DISH_SUCCESS) {
		return DISH_IS_EMPTY;
	}
	if (quality2 >= quality1) {
		*isBetter = false;
	}
	for (int i = 0; i < dish1->currentIngredients; i++) {
		cost1 += dish1->ingredients[i]->cost;
	}
	for (int i = 0; i < dish2->currentIngredients; i++) {
		cost2 += dish2->ingredients[i]->cost;
	}
	if (cost1 > cost2*(1+flexibility)) {
		*isBetter = false;
	}
	return DISH_SUCCESS;
}
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