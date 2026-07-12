#include "sounds.h"

void load_sounds(Sounds *sounds) {
	sounds->magic = LoadSound("resources/sounds/magic.wav");
	sounds->special = LoadSound("resources/sounds/special.wav");
	sounds->throw = LoadSound("resources/sounds/throw.wav");
	sounds->hit = LoadSound("resources/sounds/hit.wav");
	sounds->block = LoadSound("resources/sounds/block.wav");
	sounds->heal = LoadSound("resources/sounds/heal.wav");
	sounds->reflect = LoadSound("resources/sounds/reflect.wav");
	sounds->power = LoadSound("resources/sounds/power.wav");
	sounds->advance = LoadSound("resources/sounds/advance.wav");
	sounds->glyph = LoadSound("resources/sounds/glyph.wav");
	sounds->success = LoadSound("resources/sounds/success.wav");
	sounds->fail = LoadSound("resources/sounds/fail.wav");
	sounds->place = LoadSound("resources/sounds/place.wav");
	sounds->charge = LoadSound("resources/sounds/charge.wav");
	sounds->teleport = LoadSound("resources/sounds/teleport.wav");
	sounds->win = LoadSound("resources/sounds/win.wav");
	sounds->death = LoadSound("resources/sounds/death.wav");
}

void unload_sounds(Sounds *sounds) {
    UnloadSound(sounds->magic);
    UnloadSound(sounds->special);
    UnloadSound(sounds->throw);
    UnloadSound(sounds->hit);
    UnloadSound(sounds->block);
    UnloadSound(sounds->heal);
    UnloadSound(sounds->reflect);
    UnloadSound(sounds->power);
    UnloadSound(sounds->advance);
    UnloadSound(sounds->glyph);
    UnloadSound(sounds->success);
    UnloadSound(sounds->fail);
    UnloadSound(sounds->place);
    UnloadSound(sounds->charge);
    UnloadSound(sounds->teleport);
    UnloadSound(sounds->win);
    UnloadSound(sounds->death);
}
