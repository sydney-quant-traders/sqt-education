#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// LIBRARY CODE ////////////////////////////////////////////////////////////////

// Animal class defintion
typedef struct Animal {
    int *age_;
    int *weight_;
} Animal;

    void Animal_ctor(Animal *this, int age, int weight) {
        // dyanamic allocation of integers (heap memory)
        this->age_ = (int *) malloc(sizeof(int));
        this->weight_ = (int *) malloc(sizeof(int));

        *(this->age_) = age;
        *(this->weight_) = weight;
    }

    void Animal_dtor(Animal *this) {
        free(this->age_);
        free(this->weight_);
    }

    int Animal_get_age(Animal *this) {
        return *this->age_;
    }

    int Animal_get_weight(Animal *this) {
        return *this->weight_;
    }

    void Animal_speak(Animal *this) {
        printf("...\n");
    }
// End Animal class definition

// Dog class definition
typedef struct Dog {
    Animal base_;
    int *bark_volume_;
} Dog;

    void Dog_ctor(Dog *this, int age, int weight, int bark_volume) {
        /* TODO: implement constructor */
    }

    void Dog_dtor(Dog *this) {
        /* TODO: implement destructor */
    }

    void Dog_speak(Dog *this) {
        printf("I just woofed %d times\n", *(this->bark_volume_));
    }
// End Dog class definition

// APPLICATION CODE ////////////////////////////////////////////////////////////
// DO NOT MODIFY

int main(void) {
    {
        Dog d;
        Dog_ctor(&d, 3, 20, 5);


        assert(Animal_get_age((Animal *)&d) == 3);
        assert(Animal_get_weight((Animal *)&d) == 20);

        // ensure this call prints "I just woofed 5 times"
        Dog_speak(&d);
        
        // CAUTION: must make sure to call the Dog destructor.
        // Calling the Animal destructor would leak memory (bark_volume_)!
        Dog_dtor(&d);
    }

    {
        Animal a;
        Animal_ctor(&a, 5, 50);

        assert(Animal_get_age(&a) == 5);
        assert(Animal_get_weight(&a) == 50);

        // ensure this call prints "..."
        Animal_speak(&a);

        Animal_dtor(&a);
    }
    return 0;
}