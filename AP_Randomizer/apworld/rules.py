from BaseClasses import CollectionState
from typing import Dict, Callable, TYPE_CHECKING
from worlds.generic.Rules import add_rule, set_rule, CollectionRule
from .constants.difficulties import NORMAL

if TYPE_CHECKING:
    from . import PseudoregaliaWorld
else:
    PseudoregaliaWorld = object


class PseudoregaliaRulesHelpers:
    world: PseudoregaliaWorld
    player: int
    region_rules: dict[str, list[CollectionRule]]
    location_rules: dict[str, list[CollectionRule]]
    # Empty list or missing keys are True, any False rules need to be explicit, multiple rules are ORd together
    # Classes instantiated in difficulty order and append new clauses to rules,
    # add_rule applies them backwards meaning harder rules will shortcircuit easier rules

    required_small_keys: int = 6  # Set to 7 for Normal logic.

    def __init__(self, world: PseudoregaliaWorld) -> None:
        self.world = world
        self.player = world.player
        self.region_rules = {}
        self.location_rules = {}

        logic_level = world.options.logic_level.value
        if bool(world.options.obscure_logic):
            self.knows_obscure = lambda state: True
            self.can_attack = lambda state: self.has_breaker(state) or self.has_plunge(state)
        else:
            self.knows_obscure = lambda state: False
            self.can_attack = lambda state: self.has_breaker(state)

        if logic_level == NORMAL:
            self.required_small_keys = 7

    def apply_clauses(self, region_clauses, location_clauses):
        for name, rule in region_clauses.items():
            if name not in self.region_rules:
                self.region_rules[name] = []
            self.region_rules[name].append(rule)
        for name, rule in location_clauses.items():
            if name not in self.location_rules:
                self.location_rules[name] = []
            self.location_rules[name].append(rule)

    def has_breaker(self, state) -> bool:
        return state.has_any({"Dream Breaker", "Progressive Dream Breaker"}, self.player)

    def has_slide(self, state) -> bool:
        return state.has_any({"Slide", "Progressive Slide"}, self.player)

    def has_plunge(self, state) -> bool:
        return state.has("Sunsetter", self.player)

    def has_gem(self, state) -> bool:
        return state.has("Cling Gem", self.player)

    def can_bounce(self, state) -> bool:
        return self.has_breaker(state) and state.has("Ascendant Light", self.player)

    def can_attack(self, state) -> bool:
        """Used where either breaker or sunsetter will work, for example on switches.
        Using sunsetter is considered Obscure Logic by this method."""
        raise Exception("can_attack() was not set")

    def get_kicks(self, state, count: int) -> bool:
        kicks: int = 0
        if (state.has("Sun Greaves", self.player)):
            kicks += 3
        kicks += state.count("Heliacal Power", self.player)
        kicks += state.count("Air Kick", self.player)
        return kicks >= count

    def kick_or_plunge(self, state, count: int) -> bool:
        """Used where one air kick can be replaced with sunsetter.
        Input is the number of kicks needed without plunge."""
        total: int = 0
        if (state.has("Sun Greaves", self.player)):
            total += 3
        if (state.has("Sunsetter", self.player)):
            total += 1
        total += state.count("Heliacal Power", self.player)
        total += state.count("Air Kick", self.player)
        return total >= count

    def has_small_keys(self, state) -> bool:
        if not self.can_attack(state):
            return False
        return state.count("Small Key", self.player) >= self.required_small_keys

    def navigate_darkrooms(self, state) -> bool:
        # TODO: Update this to check obscure tricks for breaker only when logic rework nears completion
        return self.has_breaker(state) or state.has("Ascendant Light", self.player)

    def can_slidejump(self, state) -> bool:
        return (state.has_all({"Slide", "Solar Wind"}, self.player)
                or state.count("Progressive Slide", self.player) >= 2)

    def can_strikebreak(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak"}, self.player)
                or state.count("Progressive Dream Breaker", self.player) >= 2)

    def can_soulcutter(self, state) -> bool:
        return (state.has_all({"Dream Breaker", "Strikebreak", "Soul Cutter"}, self.player)
                or state.count("Progressive Dream Breaker", self.player) >= 3)

    def knows_obscure(self, state) -> bool:
        """True when Obscure Logic is enabled, False when it isn't."""
        raise Exception("knows_obscure() was not set")

    def set_pseudoregalia_rules(self) -> None:
        world = self.world
        multiworld = self.world.multiworld
        split_kicks = bool(world.options.split_sun_greaves)

        for name, rules in self.region_rules.items():
            entrance = multiworld.get_entrance(name, self.player)
            for index, rule in enumerate(rules):
                if index == 0:
                    set_rule(entrance, rule)
                else:
                    add_rule(entrance, rule, "or")
        for name, rules in self.location_rules.items():
            if name.startswith("Listless Library"):
                if split_kicks and name.endswith("Greaves"):
                    continue
                if not split_kicks and name[-1].isdigit():
                    continue
            location = multiworld.get_location(name, self.player)
            for index, rule in enumerate(rules):
                if index == 0:
                    set_rule(location, rule)
                else:
                    add_rule(location, rule, "or")

        set_rule(multiworld.get_location("D S T RT ED M M O   Y", self.player), lambda state:
                 state.has_all({
                     "Major Key - Empty Bailey",
                     "Major Key - The Underbelly",
                     "Major Key - Tower Remains",
                     "Major Key - Sansa Keep",
                     "Major Key - Twilight Theatre",
                 }, self.player))
        multiworld.completion_condition[self.player] = lambda state: state.has(
            "Something Worth Being Awake For", self.player)
