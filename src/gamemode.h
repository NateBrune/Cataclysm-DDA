#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <vector>
#include <string>
#include "enums.h"
#include "itype.h"
#include "string_id.h"

enum action_id : int;

struct special_game;
struct mtype;
using mtype_id = string_id<mtype>;

std::string special_game_name(special_game_id id);
special_game *get_special_game(special_game_id id);

struct special_game {
    virtual ~special_game() {
        return;
    };
    virtual special_game_id id() {
        return SGAME_NULL;
    };
    // init is run when the game begins
    virtual bool init() {
        return true;
    };
    // per_turn is run every turn--before any player actions
    virtual void per_turn() { };
    // pre_action is run after a keypress, but before the game handles the action
    // It may modify the action, e.g. to cancel it
    virtual void pre_action( action_id & ) { };
    // post_action is run after the game handles the action
    virtual void post_action( action_id ) { };
    // game_over is run when the player dies (or the game otherwise ends)
    virtual void game_over() { };

};

// TUTORIAL:

enum tut_lesson {
    LESSON_INTRO,
    LESSON_MOVE, LESSON_LOOK, LESSON_OPEN, LESSON_CLOSE, LESSON_SMASH,
    LESSON_WINDOW, LESSON_PICKUP, LESSON_EXAMINE, LESSON_INTERACT,

    LESSON_FULL_INV, LESSON_WIELD_NO_SPACE, LESSON_AUTOWIELD, LESSON_ITEM_INTO_INV,
    LESSON_GOT_ARMOR, LESSON_GOT_WEAPON, LESSON_GOT_FOOD, LESSON_GOT_TOOL,
    LESSON_GOT_GUN, LESSON_GOT_AMMO, LESSON_WORE_ARMOR, LESSON_WORE_STORAGE,
    LESSON_WORE_MASK,

    LESSON_WEAPON_INFO, LESSON_HIT_MONSTER, LESSON_PAIN, LESSON_BUTCHER,

    LESSON_TOOK_PAINKILLER, LESSON_TOOK_CIG, LESSON_DRANK_WATER,

    LESSON_ACT_GRENADE, LESSON_ACT_BUBBLEWRAP,

    LESSON_OVERLOADED,

    LESSON_GUN_LOAD, LESSON_GUN_FIRE, LESSON_RECOIL,

    LESSON_STAIRS, LESSON_DARK_NO_FLASH, LESSON_DARK, LESSON_PICKUP_WATER,

    NUM_LESSONS
};

struct tutorial_game : public special_game {
        virtual special_game_id id() override {
            return SGAME_TUTORIAL;
        };
        virtual bool init() override;
        virtual void per_turn() override;
        virtual void pre_action( action_id &act ) override;
        virtual void post_action( action_id act ) override;
        virtual void game_over() override { };

    private:
        void add_message( tut_lesson lesson );

        bool tutorials_seen[NUM_LESSONS];
};


// DEFENSE

enum defense_style {
    DEFENSE_CUSTOM = 0,
    DEFENSE_EASY,
    DEFENSE_MEDIUM,
    DEFENSE_HARD,
    DEFENSE_SHAUN,
    DEFENSE_DAWN,
    DEFENSE_SPIDERS,
    DEFENSE_TRIFFIDS,
    DEFENSE_SKYNET,
    DEFENSE_LOVECRAFT,
    NUM_DEFENSE_STYLES
};

enum defense_location {
    DEFLOC_NULL = 0,
    DEFLOC_HOSPITAL,
    DEFLOC_WORKS,
    DEFLOC_MALL,
    DEFLOC_BAR,
    DEFLOC_MANSION,
    NUM_DEFENSE_LOCATIONS
};

enum caravan_category {
    CARAVAN_CART = 0,
    CARAVAN_MELEE,
    CARAVAN_GUNS,
    CARAVAN_COMPONENTS,
    CARAVAN_FOOD,
    CARAVAN_CLOTHES,
    CARAVAN_TOOLS,
    NUM_CARAVAN_CATEGORIES
};

struct defense_game : public special_game {
        defense_game();

        virtual special_game_id id() override {
            return SGAME_DEFENSE;
        };
        virtual bool init() override;
        virtual void per_turn() override;
        virtual void pre_action( action_id &act ) override;
        virtual void post_action( action_id act ) override;
        virtual void game_over() override;

    private:
        void init_to_style( defense_style new_style );
        void load_style( std::string style_name );

        void setup();
        void refresh_setup( WINDOW *w, int selection );
        void init_itypes();
        void init_mtypes();
        void init_constructions();
        void init_recipes();
        void init_map();
        std::vector<itype_id> carvan_items( caravan_category cat );

        void spawn_wave();
        void caravan();
        std::vector<mtype_id> pick_monster_wave();
        void spawn_wave_monster( const mtype_id &type );

        std::string special_wave_message( std::string name );


        // DATA
        int current_wave;

        defense_style style;       // What type of game is it?
        defense_location location; // Where are we?

        int initial_difficulty; // Total "level" of monsters in first wave
        int wave_difficulty;    // Increased "level" of monsters per wave

        int time_between_waves;     // Cooldown / building / healing time
        int waves_between_caravans; // How many waves until we get to trade?

        unsigned long initial_cash;  // How much cash do we start with?
        unsigned long cash_per_wave; // How much cash do we get per wave?
        unsigned long cash_increase; // How much does the above increase per wave?

        bool zombies;
        bool specials;
        bool spiders;
        bool triffids;
        bool robots;
        bool subspace;

        bool hunger; // Do we hunger?
        bool thirst; // Do we thirst?
        bool sleep;  // Do we need to sleep?

        bool mercenaries; // Do caravans offer the option of hiring a mercenary?

};

#endif
