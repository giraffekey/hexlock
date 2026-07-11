#include "player.h"

const char* get_hex_name(Hex hex) {
    if (hex.valid) {
        switch (hex.sign + hex.omega * N_SIGNS) {
        case ARIES: return "Aries";
        case ARIES + N_SIGNS: return "AriesΩ";
        case TAURUS: return "Taurus";
        case TAURUS + N_SIGNS: return "TaurusΩ";
        case GEMINI: return "Gemini";
        case GEMINI + N_SIGNS: return "GeminiΩ";
        case CANCER: return "Cancer";
        case CANCER + N_SIGNS: return "CancerΩ";
        case LEO: return "Leo";
        case LEO + N_SIGNS: return "LeoΩ";
        case VIRGO: return "Virgo";
        case VIRGO + N_SIGNS: return "VirgoΩ";
        case LIBRA: return "Libra";
        case LIBRA + N_SIGNS: return "LibraΩ";
        case SCORPIO: return "Scorpio";
        case SCORPIO + N_SIGNS: return "ScorpioΩ";
        case SAGITTARIUS: return "Sagittarius";
        case SAGITTARIUS + N_SIGNS: return "SagittariusΩ";
        case CAPRICORN: return "Capricorn";
        case CAPRICORN + N_SIGNS: return "CapricornΩ";
        case AQUARIUS: return "Aquarius";
        case AQUARIUS + N_SIGNS: return "AquariusΩ";
        case PISCES: return "Pisces";
        case PISCES + N_SIGNS: return "PiscesΩ";
        }
    } else {
        return "";
    }
}

const char* get_hex_desc(Hex hex) {
    if (hex.valid) {
        switch (hex.sign + hex.omega * N_SIGNS) {
        case ARIES: return "High damage fire wave";
        case ARIES + N_SIGNS: return "High damage fire blast";
        case TAURUS: return "Place rock to block spells";
        case TAURUS + N_SIGNS: return "Place crystal to reflect spells";
        case GEMINI: return "Move faster";
        case GEMINI + N_SIGNS: return "Move and cast faster";
        case CANCER: return "AOE whirlpool";
        case CANCER + N_SIGNS: return "AOE hurricane";
        case LEO: return "Cast fireball when hit";
        case LEO + N_SIGNS: return "Cast wildfire when hit";
        case VIRGO: return "Advance 1 tile";
        case VIRGO + N_SIGNS: return "Advance 1 column";
        case LIBRA: return "Diagonally moving twister";
        case LIBRA + N_SIGNS: return "Two diagonally moving twisters";
        case SCORPIO: return "Heal from next attack";
        case SCORPIO + N_SIGNS: return "Heal from and reflect next attack";
        case SAGITTARIUS: return "Place magma on enemy field";
        case SAGITTARIUS + N_SIGNS: return "Fill enemy row with magma";
        case CAPRICORN: return "Shockwave in one row";
        case CAPRICORN + N_SIGNS: return "Earthquake in every row";
        case AQUARIUS: return "Enter phantom mode";
        case AQUARIUS + N_SIGNS: return "Enter phantom mode longer";
        case PISCES: return "Restore 1 HP";
        case PISCES + N_SIGNS: return "Restore all HP";
        }
    } else {
        return "";
    }
}

Hex find_hex(SignU8 signs[]) {
    SignU8 max_sign = 0;
    uint8_t occurences[N_SIGNS] = {0};
    for (size_t i = 0; i < MAX_SIGNS; ++i) {
        SignU8 sign = signs[i];
        occurences[sign]++;
        if (occurences[sign] > occurences[max_sign]) {
            max_sign = sign;
        }
    }

    switch (occurences[max_sign]) {
    case 2:
        return (Hex){true, max_sign, false};
    case 3:
        return (Hex){true, max_sign, true};
    default:
        return (Hex){0};
    }
}

void add_hex(Player *player, Hex hex) {
    player->hexes[player->n_hexes % MAX_HEXES] = hex;
    player->n_hexes++;
}

void use_hex(Player *player) {
    player->n_hexes = min(player->n_hexes, MAX_HEXES) - 1;
    for (size_t i = 0; i < player->n_hexes; ++i) {
        player->hexes[i] = player->hexes[i + 1];
    }
    player->hexes[player->n_hexes].valid = false;
}

void damage_player(Player *player, uint8_t damage) {
    if (!player->statuses[STATUS_PHANTOM]) {
        if (damage >= player->hp) player->hp = 0;
        else player->hp -= damage;
    }
}
