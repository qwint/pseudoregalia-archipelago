from .rules import PseudoregaliaRulesHelpers


class PseudoregaliaNormalRules(PseudoregaliaRulesHelpers):
    def __init__(self, world) -> None:
        super().__init__(world)
        upper_bailey = world.get_region("Upper Bailey")

        region_clauses = {
            "Empty Bailey -> Upper Bailey": lambda state:
                self.has_slide(state) and self.can_attack(state) and self.kick_or_plunge(state, 1)  # going through inside building to reach the tip
                or self.get_kicks(state, 3)  # 2 is possible but a bit tight, 1 is possible with a funky jump
                or self.has_slide(state)  # ultras
                or self.has_plunge(state)
                or self.has_gem(state)
                or self.can_bounce(state),  # annoying projectile pogo
            # "Empty Bailey -> Castle Main": lambda state: True,
            # "Empty Bailey -> Bailey => Theatre Pillar": lambda state: True,
            # "Upper Bailey -> Empty Bailey": lambda state: True,
            "Upper Bailey -> Tower Remains": lambda state:
                (
                    # get into the tower
                    self.kick_or_plunge(state, 4)
                    or self.has_slide(state))  # ultras, is a bit tight, consider adding a kick to make it easier?
                and (
                # get onto the bridge
                    self.can_slidejump(state)
                    or self.has_slide(state)  # ultras
                    or self.get_kicks(state, 3)  # easiest
                    or self.get_kicks(state, 2)  # pretty doable
                    or self.get_kicks(state, 1)  # hard but possible
                    or self.has_gem(state)  # lunatic behaviour that i'm not trying
                    or self.has_plunge(state)),  # crouch backflip tech
            "Tower Remains -> Underbelly Little Guy": lambda state:
                self.has_plunge(state),  # nothing better
            "Tower Remains -> The Great Door": lambda state:
                # get to top of tower
                self.has_slide(state) # ultras from right tower directly to pole
                and (
                    self.has_gem(state)
                    or self.kick_or_plunge(state, 2))  # double check 1 kick + plunge works, should be doable with 1 kick on lunatic?
                or self.can_attack(state) and self.has_gem(state),  # open left tower and climb it, consider adding a kick or plunge to make the first left pole easier
           
            "Theatre Main -> Keep Main": lambda state:
                self.has_gem(state),
            "Theatre Main -> Theatre Pillar": lambda state:
                self.kick_or_plunge(state, 2)
                or self.get_kicks(state, 1)  # technical
                or self.has_gem(state),
            "Theatre Main -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.has_slide(state)  # ultrahops
                or self.can_slidejump(state),
            "Bailey => Theatre Pillar -> Theatre Pillar": lambda state: 
                self.has_plunge(state)
                or self.has_slide(state)
                or self.can_slidejump(state)
                or self.get_kicks(state, 1)  # expert?
                # or self.get_kicks(state, 3)  # dunno none of these feel easier than 1kick
                or self.get_kicks(state, 1) and self.can_bounce(state),
            "Bailey => Theatre Pillar -> Empty Bailey": lambda state: True,
            "Castle => Theatre Pillar -> Theatre Pillar": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 1)  # expert?
                or self.has_slide(state)
                or self.can_slidejump(state),
            "Castle => Theatre Pillar -> Castle Main": lambda state: 
                True  # harder difficulties
                or self.kick_or_plunge(state, 1)
                or self.has_slide(state)
                or self.can_slidejump(state)
                or self.has_gem(state),
            "Theatre Pillar -> Theatre Main": lambda state:
                (  # get up ledge
                    self.has_gem(state)
                    or self.has_slide(state) and self.kick_or_plunge(state, 3)
                    or self.can_slidejump(state) and self.kick_or_plunge(state, 3)
                    or self.has_plunge(state) and self.get_kicks(state, 3))
                    # or whatever vanilla bubble use is
                and ( # cross lava
                    self.kick_or_plunge(state, 2)
                    or self.get_kicks(state, 1)  # technical
                    or self.has_gem(state)),
            "Theatre Pillar -> Bailey => Theatre Pillar": lambda state: True,
            "Theatre Pillar -> Castle => Theatre Pillar": lambda state: True,
            "Theatre Outside Scythe Corridor -> Theatre Main": lambda state:
                self.has_gem(state) and self.get_kicks(state, 3)
                or self.has_gem(state) and self.can_slidejump(state),

            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            # "Dungeon => Castle -> Dungeon Mirror": lambda state: True,
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
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
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            # "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state: True,
            # "Castle Main -> Dungeon => Castle": lambda state: True,
            # "Castle Main -> Keep Main": lambda state: True,
            # "Castle Main -> Empty Bailey": lambda state: True,
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
            "Castle => Theatre (Front) -> Theatre Main": lambda state:  # TODO make sure to difficulty split
                self.has_gem(state)
                or self.has_slide(state)  # ultra
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3),
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
            "Keep Main -> Keep => Underbelly": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Keep Main -> Theatre Outside Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_bounce(state)
                or self.can_slidejump(state),
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
                self.has_breaker(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 4))
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Underbelly Ascendant Light -> Underbelly Light Pillar": lambda state:
                self.has_breaker(state),
            "Underbelly Ascendant Light -> Underbelly => Dungeon": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state),
            # "Underbelly Main Lower -> Underbelly Little Guy": lambda state: True,
            "Underbelly Main Lower -> Underbelly Hole": lambda state:
                self.has_plunge(state)
                and (
                    self.get_kicks(state, 1)
                    or self.can_slidejump(state)
                    or self.can_attack(state)),
            "Underbelly Main Lower -> Underbelly By Heliacal": lambda state:
                self.has_slide(state) and self.has_plunge(state),
            "Underbelly Main Lower -> Underbelly Main Upper": lambda state:
                self.has_plunge(state)
                and (
                    self.get_kicks(state, 2)
                    or self.get_kicks(state, 1) and self.has_gem(state)),
            # "Underbelly Main Upper -> Underbelly Main Lower": lambda state: True,
            "Underbelly Main Upper -> Underbelly Light Pillar": lambda state:
                self.has_breaker(state) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 2)
                or self.has_gem(state)
                and (
                    self.get_kicks(state, 2) and self.has_plunge(state)
                    or self.get_kicks(state, 4)),
            "Underbelly Main Upper -> Underbelly By Heliacal": lambda state:
                self.has_breaker(state)
                and (
                    state.has("Ascendant Light", self.player)
                    or self.can_slidejump(state) and self.get_kicks(state, 3)
                    or self.has_gem(state) and self.get_kicks(state, 2)),
            "Underbelly By Heliacal -> Underbelly Main Upper": lambda state:
                self.has_breaker(state) and self.has_plunge(state)
                or self.knows_obscure(state) and self.has_plunge(state)
                and (
                    self.get_kicks(state, 1)
                    or self.has_gem(state)),
            # "Underbelly Little Guy -> Empty Bailey": lambda state: True,
            "Underbelly Little Guy -> Upper Bailey": lambda state:  # TODO make sure to difficulty split
                self.knows_obscure(state)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_slide(state)  # ultras
                or self.can_slidejump(state),
            "Underbelly Little Guy -> Underbelly Main Lower": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),
            # "Underbelly => Keep -> Keep => Underbelly": lambda state: True,
            "Underbelly => Keep -> Underbelly Hole": lambda state:
                self.has_plunge(state),
            "Underbelly Hole -> Underbelly Main Lower": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.can_slidejump(state)
                or self.can_attack(state),
            "Underbelly Hole -> Underbelly => Keep": lambda state:
                self.has_slide(state),
        }

        location_clauses = {
            "Empty Bailey - Solar Wind": lambda state:
                self.has_slide(state),  # to consider: damage boosting w/ crouch
            "Empty Bailey - Cheese Bell": lambda state:
                ( # to get to ledge assuming top of bailey access
                    self.can_slidejump(state)  # i'm just assuming here
                    or self.has_plunge(state)  # crouch backflip tech
                    or self.has_slide(state)  # ultras
                    or self.get_kicks(state, 1))
                    # or self.can_bounce(state)  # doable but you need one of the others to get across that one gap so who cares
                and ( # to get from ledge to bell
                    self.has_slide(state)  # ultras
                    and (
                        self.kick_or_plunge(state, 2)
                        or self.has_gem(state))
                    or self.can_slidejump(state)  # untested
                    and (
                        self.kick_or_plunge(state, 2)
                        or self.has_gem(state))
                    or self.has_gem(state)  # not that tight tbh but maybe annoying enough to remove from normal
                    or self.get_kicks(state, 3)),  # not that tight but requires a backflip (which should be normal)
            "Empty Bailey - Inside Building": lambda state:
                self.has_slide(state),  # unchanged by difficulty
            "Empty Bailey - Center Steeple": lambda state:
                self.has_plunge(state)  # a little tight but probably normal
                or self.get_kicks(state, 3)  # 2 is possible but a bit tight, 1 is possible with the tables + a funky jump to get on top floor
                or self.has_slide(state),  # ultras
            "Empty Bailey - Guarded Hand": lambda state:
                upper_bailey.can_reach(state)
                and (
                    self.knows_obscure(state)
                    or self.has_gem(state)
                    or self.get_kicks(state, 4))  # doable with kicks 0-4 tbh
                or self.has_breaker(state)
                and (   # do the fight
                    self.has_plunge(state)
                    or self.get_kicks(state, 2)),
            "Twilight Theatre - Soul Cutter": lambda state:
                self.can_strikebreak(state)
                and (  # we probably already have some of this movement but worth marking it imo
                    self.can_bounce(state)
                    or self.kick_or_plunge(state, 1)
                    or self.has_gem(state)
                    or self.has_slide(state)  # ultras
                    or self.can_slidejump(state)),
            "Twilight Theatre - Corner Beam": lambda state:
                self.has_gem(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 1)  # tight, 2 makes it pretty easy
                    or self.get_kicks(state, 2)
                    or self.can_slidejump(state)
                    or self.has_slide(state))  # ultras
                or self.has_plunge(state) and get_kicks(state, 3)  # use crouch backflip
                or self.get_kicks(state, 4)  # easy just slam kicks down
                or self.has_slide(state) and self.kick_or_plunge(state, 2),  # ultra
            "Twilight Theatre - Locked Door": lambda state:
                self.has_small_keys(state)
                and (
                    self.has_gem(state)
                    or self.can_bounce(state)
                    or self.get_kicks(state, 3)
                    or self.has_plunge(state) and self.get_kicks(state, 1)  # crouch backflip from doorway
                    or self.has_slide(state)  # ultra
                    or self.can_slidejump(state)  # probably, idk
                    ),
            "Twilight Theatre - Back Of Auditorium": lambda state:
                self.has_plunge(state)  # annoying to get up and uses crouch backflips afterwards
                or self.get_kicks(state, 1)  # babee route works
                or self.has_gem(state)
                or self.has_slide(state)  # super annoying ultrahops
                or self.can_slidejump(state),  # shit maybe idk
            "Twilight Theatre - Murderous Goat": lambda state: True,
            "Twilight Theatre - Center Stage": lambda state:  # i don't feel super confident about this
                self.can_soulcutter(state) and self.has_gem(state) and self.kick_or_plunge(state, 1) and self.can_slidejump(state),
                # TODO
                # leftside: soulcutter+(cling|3kickor)
                # rightside: cling only technical but probably add some vertical for nicety, plus whatever the clingless lunatic route is
                # middle: (getting to back area needs: cling, 5kickor, scythe entrance)
                    # silly doorframe shit: cling
                    # shortcut: ultra + 1kickor + cling
                    # arena: breaker + (3kickor, cling)
            "Tower Remains - Cling Gem": lambda state:
                # get to gem by crossing the gap
                self.can_slidejump(state)  # pretty sure this is easy but can't test rn
                or self.has_slide(state)  # ultras
                or self.kick_or_plunge(state, 2)  # climb the right tower and cross
                or self.has_gem(state), # ride from back of right tower to ledge, a little tight(?)

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
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.has_gem(state) and self.get_kicks(state, 3)),
            # "Castle Sansa - Indignation": lambda state: True,
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
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
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
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
                (self.can_attack(state))
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
            "Sansa Keep - Lonely Throne": lambda state:
                self.has_breaker(state) and self.has_gem(state)
                and (
                    self.has_plunge(state) and self.get_kicks(state, 1)
                    or self.has_plunge(state) and self.can_bounce(state)
                    or self.get_kicks(state, 1) and self.can_bounce(state))
                or self.can_bounce(state) and self.kick_or_plunge(state, 4),
            # "The Underbelly - Ascendant Light": lambda state: True,
            "The Underbelly - Rafters Near Keep": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 2)
                or self.can_bounce(state),
            "The Underbelly - Locked Door": lambda state:
                self.has_small_keys(state),
            "The Underbelly - Main Room": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.can_slidejump(state) and self.get_kicks(state, 1),
            "The Underbelly - Alcove Near Light": lambda state:
                self.can_attack(state)
                or self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 3) and self.can_slidejump(state),
            "The Underbelly - Building Near Little Guy": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 3),
            "The Underbelly - Strikebreak Wall": lambda state:
                self.can_strikebreak(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 4)
                    or self.get_kicks(state, 2) and self.has_plunge(state)),
            "The Underbelly - Surrounded By Holes": lambda state:
                self.can_soulcutter(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 2))
                or self.can_slidejump(state) and self.has_gem(state) and self.get_kicks(state, 1),
        }

        self.apply_clauses(region_clauses, location_clauses)

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
