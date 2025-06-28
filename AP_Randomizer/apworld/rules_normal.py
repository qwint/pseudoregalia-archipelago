from .rules import PseudoregaliaRulesHelpers
from .constants.versions import MAP_PATCH


class PseudoregaliaNormalRules(PseudoregaliaRulesHelpers):
    def __init__(self, world) -> None:
        super().__init__(world)
        upper_bailey = world.get_region("Bailey Upper")

        region_clauses = {
            "Bailey Lower -> Bailey Upper": lambda state:
                self.has_slide(state) and self.can_attack(state)  # going through inside building to reach the tip
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.knows_obscure(state),
            # "Bailey Lower -> Castle Main": lambda state: True,
            # "Bailey Lower -> Theatre Pillar => Bailey": lambda state: True,
            # "Bailey Upper -> Bailey Lower": lambda state: True,
            "Bailey Upper -> Underbelly => Bailey": lambda state:
                self.has_plunge(state),
            "Tower Remains -> The Great Door": lambda state:
                self.can_attack(state) and self.has_gem(state) and self.kick_or_plunge(state, 1),
            "Theatre Main -> Theatre Outside Scythe Corridor": lambda state:
                self.has_gem(state),
            "Theatre Main -> Theatre Pillar": lambda state:
                self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.has_plunge(state) and self.knows_obscure(state)
                or self.has_gem(state),
            "Theatre Main -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state),
            "Theatre Pillar => Bailey -> Theatre Pillar": lambda state:
                self.has_plunge(state) and self.knows_obscure(state)
                or self.get_kicks(state, 1) and self.can_bounce(state),
            # "Theatre Pillar => Bailey -> Bailey Lower": lambda state: True,
            "Castle => Theatre Pillar -> Theatre Pillar": lambda state:
                self.has_plunge(state),
            # "Castle => Theatre Pillar -> Castle Main": lambda state: True,
            "Theatre Pillar -> Theatre Main": lambda state:
                self.has_gem(state)
                or self.has_plunge(state) and self.get_kicks(state, 3),
            # "Theatre Pillar -> Theatre Pillar => Bailey": lambda state: True,
            # "Theatre Pillar -> Castle => Theatre Pillar": lambda state: True,
            "Theatre Outside Scythe Corridor -> Theatre Main": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),
            "Theatre Outside Scythe Corridor -> Dungeon Escape Upper": lambda state:
                self.navigate_darkrooms(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 1)
                    or self.has_gem(state)
                    or self.can_slidejump(state)
                    or self.knows_obscure(state) and self.has_plunge(state)),
            "Theatre Outside Scythe Corridor -> Keep Main": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_bounce(state)
                or self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),

            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.knows_obscure(state) and self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            # "Dungeon => Castle -> Dungeon Mirror": lambda state: True,
            # "Dungeon => Castle -> Castle Main": lambda state: True,
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.get_kicks(state, 1) and self.has_plunge(state)
                or self.get_kicks(state, 3),
            # "Dungeon Escape Lower -> Underbelly => Dungeon": lambda state: True,
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.get_kicks(state, 1)
                or self.has_gem(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            # "Castle Main -> Dungeon => Castle": lambda state: True,
            # "Castle Main -> Keep Main": lambda state: True,
            # "Castle Main -> Bailey Lower": lambda state: True,
            "Castle Main -> Library Main": lambda state:
                self.can_attack(state),
            "Castle Main -> Castle => Theatre Pillar": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state),
            # "Castle Spiral Climb -> Castle Main": lambda state: True,
            "Castle Spiral Climb -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.can_attack(state) and self.get_kicks(state, 1),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4) and self.has_plunge(state),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state)
                or self.get_kicks(state, 1) and self.has_plunge(state) and self.can_slidejump(state),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 4),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Theatre Main": lambda state:
                self.has_plunge(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 2),
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.kick_or_plunge(state, 4)
                or self.knows_obscure(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.get_kicks(state, 3) and self.can_bounce(state),
            "Keep Main -> Keep Locked Room": lambda state:
                # Note for trackers: This is accessible with nothing but not in logic.
                # Cutting the platform or hitting the lever make this harder and are irreversible.
                # On Hard and above, the player is expected to not do either.
                self.has_small_keys(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1),
            "Keep Main -> Keep Sunsetter": lambda state:
                # See "Keep Main -> Keep Locked Room".
                # All other methods would go through Keep Locked Room instead.
                self.has_gem(state),
            "Keep Main -> Keep Throne Room": lambda state:
                self.has_breaker(state) and self.has_gem(state)
                and (
                    self.has_plunge(state) and self.get_kicks(state, 1)
                    or self.has_plunge(state) and self.can_bounce(state)
                    or self.get_kicks(state, 1) and self.can_bounce(state))
                or self.can_bounce(state) and self.kick_or_plunge(state, 4),
            "Keep Main -> Keep => Underbelly": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Keep Main -> Theatre Outside Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_bounce(state)
                or self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            # "Keep Locked Room -> Keep Sunsetter": lambda state: True,
            # "Keep => Underbelly -> Keep Main": lambda state: True,
            # "Keep => Underbelly -> Underbelly => Keep": lambda state: True,
            "Underbelly => Dungeon -> Dungeon Escape Lower": lambda state:
                self.navigate_darkrooms(state),
            # "Underbelly => Dungeon -> Underbelly Light Pillar": lambda state: True,
            "Underbelly => Dungeon -> Underbelly Ascendant Light": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state)
                or self.knows_obscure(state) and self.can_attack(state),
            # "Underbelly Light Pillar -> Underbelly Main Upper": lambda state: True,
            "Underbelly Light Pillar -> Underbelly => Dungeon": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 4),
            "Underbelly Light Pillar -> Underbelly Ascendant Light": lambda state:
                # this route is more difficult with ascendant light because of the long room with the switches, but
                # since it's possible to use AL to go up the pillar and get to the item that way (i.e. passing through
                # Underbelly => Dungeon), the logic for this entrance is written assuming the player doesn't have AL
                self.has_breaker(state)
                and (
                    self.has_plunge(state)
                    or self.knows_obscure(state) and self.get_kicks(state, 3)),
            "Underbelly Ascendant Light -> Underbelly => Dungeon": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state),
            # "Underbelly Main Lower -> Underbelly => Bailey": lambda state: True,
            "Underbelly Main Lower -> Underbelly Hole": lambda state:
                self.has_plunge(state)
                and (
                    self.get_kicks(state, 1)
                    or self.can_slidejump(state)
                    or self.can_attack(state)),
            "Underbelly Main Lower -> Underbelly By Heliacal": lambda state:
                self.has_slide(state) and self.has_plunge(state),
            "Underbelly Main Lower -> Underbelly Main Upper": lambda state:
                self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 2),
            # "Underbelly Main Upper -> Underbelly Main Lower": lambda state: True,
            "Underbelly Main Upper -> Underbelly Light Pillar": lambda state:
                self.has_breaker(state) and self.has_plunge(state)
                or self.knows_obscure(state) and self.has_breaker(state)
                and (
                    self.get_kicks(state, 2)
                    or self.has_gem(state) and self.get_kicks(state, 1)),
            "Underbelly Main Upper -> Underbelly By Heliacal": lambda state:
                self.has_breaker(state)
                and (
                    state.has("Ascendant Light", self.player) and self.get_kicks(state, 1)
                    or self.can_slidejump(state) and self.get_kicks(state, 3)
                    or self.has_gem(state) and self.get_kicks(state, 2)),
            "Underbelly By Heliacal -> Underbelly Main Upper": lambda state:
                self.has_breaker(state) and self.has_plunge(state)
                or self.knows_obscure(state) and self.has_plunge(state)
                and (
                    self.get_kicks(state, 1)
                    or self.has_gem(state)),
            # "Underbelly => Bailey -> Bailey Lower": lambda state: True,
            "Underbelly => Bailey -> Bailey Upper": lambda state:
                self.knows_obscure(state)
                or self.has_plunge(state) and self.get_kicks(state, 1),
            "Underbelly => Bailey -> Underbelly Main Lower": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 2)
                or self.knows_obscure(state),
            # "Underbelly => Keep -> Keep => Underbelly": lambda state: True,
            "Underbelly => Keep -> Underbelly Hole": lambda state:
                self.has_plunge(state),
            "Underbelly Hole -> Underbelly Main Lower": lambda state:
                self.has_plunge(state)
                and (
                    self.can_attack(state)
                    or self.can_slidejump(state) and self.has_gem(state)
                    or self.can_slidejump(state) and self.get_kicks(state, 1)),
            "Underbelly Hole -> Underbelly => Keep": lambda state:
                self.has_plunge(state) and self.has_slide(state),
        }

        location_clauses = {
            "Empty Bailey - Solar Wind": lambda state:
                self.has_slide(state),  # to consider: damage boosting w/ crouch
            "Empty Bailey - Cheese Bell": lambda state:  # TODO consider to/from center steeple
                self.get_kicks(state, 3)
                or self.has_gem(state) and self.kick_or_plunge(state, 2),
            "Empty Bailey - Inside Building": lambda state:
                self.has_slide(state),
            "Empty Bailey - Center Steeple": lambda state:
                self.has_plunge(state),
            "Empty Bailey - Guarded Hand": lambda state:
                upper_bailey.can_reach(state)
                and (
                    self.knows_obscure(state)
                    or self.has_gem(state)
                    or self.get_kicks(state, 3))
                or self.has_breaker(state)   # do the fight
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 2)),
            "Twilight Theatre - Soul Cutter": lambda state:
                self.can_strikebreak(state)
                and (  # we probably already have some of this movement but worth marking it imo
                    self.can_bounce(state)
                    or self.kick_or_plunge(state, 1)
                    or self.has_gem(state)),
            "Twilight Theatre - Corner Beam": lambda state:
                self.has_gem(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 2))
                or self.has_plunge(state) and self.get_kicks(state, 3) and self.knows_obscure(state)  # use crouch backflip
                or self.get_kicks(state, 4),
            "Twilight Theatre - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 1)),
            "Twilight Theatre - Back Of Auditorium": lambda state:
                self.has_plunge(state) and self.knows_obscure(state)
                or self.get_kicks(state, 1)
                or self.has_gem(state),
            # "Twilight Theatre - Murderous Goat": lambda state: True,
            "Twilight Theatre - Center Stage": lambda state:
                self.can_soulcutter(state) and self.has_gem(state)
                and self.has_plunge(state) and self.can_slidejump(state),  # cross the gap on right side
                # potentially more routes
            "Tower Remains - Cling Gem": lambda state:
                self.kick_or_plunge(state, 2),  # climb the right tower and cross
            # "Tower Remains - Atop The Tower": lambda state: True,

            # "Dilapidated Dungeon - Dream Breaker": lambda state: True,
            # "Dilapidated Dungeon - Slide": lambda state: True,
            # "Dilapidated Dungeon - Alcove Near Mirror": lambda state: True,
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state) and self.can_bounce(state)
                or self.has_gem(state) and self.kick_or_plunge(state, 3)
                or self.get_kicks(state, 2) and self.can_bounce(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1) and self.can_bounce(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.kick_or_plunge(state, 3)
                or self.knows_obscure(state) and self.can_bounce(state) and self.has_gem(state),
            # "Castle Sansa - Indignation": lambda state: True,
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.get_kicks(state, 4)
                or self.knows_obscure(state) and self.can_bounce(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_gem(state) and self.get_kicks(state, 1),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 2),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.can_attack(state) and self.get_kicks(state, 1),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.kick_or_plunge(state, 4),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Listless Library - Sun Greaves": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.can_attack(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.can_attack(state),
            "Listless Library - Upper Back": lambda state:
                self.can_attack(state)
                and (
                    self.has_gem(state) and self.kick_or_plunge(state, 1)
                    or self.kick_or_plunge(state, 2)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.kick_or_plunge(state, 3),
            "Sansa Keep - Near Theatre": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            # "Sansa Keep - Alcove Near Locked Door": lambda state: True,
            "Sansa Keep - Levers Room": lambda state:
                self.can_attack(state),
            "Sansa Keep - Sunsetter": lambda state:
                self.can_attack(state),
            "Sansa Keep - Strikebreak": lambda state:
                self.has_breaker(state)
                and (
                    self.has_slide(state)
                    or self.can_strikebreak(state))
                and (
                    self.has_gem(state)
                    or self.has_plunge(state) and self.get_kicks(state, 1)
                    or self.get_kicks(state, 3)),
            # "Sansa Keep - Lonely Throne": lambda state: True,
            # "The Underbelly - Ascendant Light": lambda state: True,
            "The Underbelly - Rafters Near Keep": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 2)
                or self.can_bounce(state),
            "The Underbelly - Locked Door": lambda state:
                self.has_small_keys(state),
            "The Underbelly - Main Room": lambda state:
                self.has_plunge(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 2)),
            "The Underbelly - Alcove Near Light": lambda state:
                self.can_attack(state)
                or self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 3) and self.can_slidejump(state),
            "The Underbelly - Building Near Little Guy": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 3),
            "The Underbelly - Strikebreak Wall": lambda state:
                self.can_strikebreak(state) and self.can_bounce(state) and self.kick_or_plunge(state, 1),
            "The Underbelly - Surrounded By Holes": lambda state:
                self.can_soulcutter(state) and self.has_plunge(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 2)
                    or self.knows_obscure(state) and self.get_kicks(state, 1)),

            "Dilapidated Dungeon - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state),
            "Castle Sansa - Time Trial": lambda state:
                self.has_small_keys(state),
            "Sansa Keep - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state) and self.can_bounce(state),
            "Listless Library - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.has_gem(state),
            "Twilight Theatre - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state),
            "Empty Bailey - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state),
            "The Underbelly - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state),
            "Tower Remains - Time Trial": lambda state:
                self.has_breaker(state) and self.has_plunge(state) and self.get_kicks(state, 3)
                and self.has_gem(state) and self.can_slidejump(state),

            # "Castle Sansa - Memento": lambda state: True,
        }

        # logic differences due to geometry changes between versions
        if self.world.options.game_version == MAP_PATCH:
            region_clauses["Bailey Upper -> Tower Remains"] = (lambda state:
                (self.kick_or_plunge(state, 4)
                or self.get_kicks(state, 1) and self.has_plunge(state) and self.can_bounce(state))
                and (
                    # get onto the bridge
                    self.can_slidejump(state)
                    or self.has_plunge(state) and self.knows_obscure(state)))
            region_clauses["Dungeon => Castle -> Dungeon Strong Eyes"] = (lambda state:
                self.has_small_keys(state)
                or self.knows_obscure(state)
                and (
                    self.has_plunge(state)
                    or self.has_breaker(state) and self.get_kicks(state, 1)))
            region_clauses["Dungeon Strong Eyes -> Dungeon => Castle"] = (lambda state:
                self.has_small_keys(state)
                or self.knows_obscure(state)
                and (
                    self.can_bounce(state)
                    or self.can_attack(state) and self.get_kicks(state, 2)
                    or self.can_attack(state) and self.has_gem(state)))
            location_clauses["Dilapidated Dungeon - Strong Eyes"] = (lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 1)
                    or self.has_plunge(state)))
        else:
            region_clauses["Bailey Upper -> Tower Remains"] = (lambda state:
                self.kick_or_plunge(state, 4)
                and (
                    # get onto the bridge
                    self.can_slidejump(state)
                    or self.has_plunge(state) and self.knows_obscure(state)))
            region_clauses["Dungeon => Castle -> Dungeon Strong Eyes"] = (lambda state:
                self.has_small_keys(state))
            region_clauses["Dungeon Strong Eyes -> Dungeon => Castle"] = (lambda state:
                self.has_small_keys(state))
            location_clauses["Dilapidated Dungeon - Strong Eyes"] = (lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.has_gem(state) and self.get_kicks(state, 3)))

        self.apply_clauses(region_clauses, location_clauses)

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
