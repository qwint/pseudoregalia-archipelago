from .rules_hard import PseudoregaliaHardRules


class PseudoregaliaExpertRules(PseudoregaliaHardRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        region_clauses = {
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.has_slide(state) and self.get_kicks(state, 1),
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.has_slide(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_slide(state),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.has_slide(state),
            "Castle Spiral Climb -> Castle High Climb": lambda state:
                self.has_slide(state)
                or self.get_kicks(state, 2),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.kick_or_plunge(state, 4),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_slide(state) and self.get_kicks(state, 2),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_slide(state)
                or self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_slide(state)
                or self.get_kicks(state, 3),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.has_plunge(state)
                or self.has_slide(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_slide(state),
            "Library Top -> Library Greaves": lambda state:
                self.get_kicks(state, 2),
            "Keep Main -> Keep => Underbelly": lambda state:
                self.has_slide(state),
            "Keep Main -> Theatre Outside Scythe Corridor": lambda state:
                self.has_slide(state),
            "Underbelly => Dungeon -> Underbelly Ascendant Light": lambda state:
                self.has_breaker(state)
                or self.get_kicks(state, 1) and self.has_slide(state),
            "Underbelly Light Pillar -> Underbelly => Dungeon": lambda state:
                self.has_slide(state) and self.kick_or_plunge(state, 2),
            "Underbelly Light Pillar -> Underbelly Ascendant Light": lambda state:
                self.has_breaker(state)
                and (
                    self.get_kicks(state, 2)
                    or self.get_kicks(state, 1) and self.has_gem(state)
                    or self.has_slide(state))
                or self.has_plunge(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 1)
                    or self.has_slide(state)),
            "Underbelly Ascendant Light -> Underbelly => Dungeon": lambda state:
                self.has_slide(state) and self.get_kicks(state, 1),
            "Underbelly Main Lower -> Underbelly Hole": lambda state:
                self.has_plunge(state) and self.has_slide(state),
            "Underbelly Main Lower -> Underbelly By Heliacal": lambda state:
                self.has_slide(state),
            "Underbelly Main Lower -> Underbelly Main Upper": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 4)
                or self.has_slide(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 3)
                    or self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.get_kicks(state, 1) and self.has_breaker(state)),
            "Underbelly Main Upper -> Underbelly Light Pillar": lambda state:
                self.has_breaker(state) and self.has_slide(state)
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.has_plunge(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.get_kicks(state, 2),
            "Underbelly Main Upper -> Underbelly By Heliacal": lambda state:
                self.has_breaker(state) and self.has_slide(state) and self.get_kicks(state, 2),
            "Underbelly By Heliacal -> Underbelly Main Upper": lambda state:
                self.has_plunge(state)
                or self.has_breaker(state)
                and (
                    self.has_slide(state)
                    or self.has_gem(state)
                    or self.get_kicks(state, 1))
                or self.has_slide(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 2)),
            "Underbelly Hole -> Underbelly Main Lower": lambda state:
                self.has_slide(state),
        }

        location_clauses = {
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_slide(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.can_bounce(state),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.kick_or_plunge(state, 2)
                or self.can_bounce(state) and self.get_kicks(state, 1)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_gem(state)
                or self.has_slide(state) and self.get_kicks(state, 1),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state)
                and (
                    self.kick_or_plunge(state, 1)
                    or self.has_slide(state))
                or self.has_slide(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 3)
                or self.has_gem(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.has_slide(state),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_slide(state),
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Balcony": lambda state:
                self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Corner Corridor": lambda state:
                self.get_kicks(state, 2) and self.has_slide(state),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.kick_or_plunge(state, 3)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.has_slide(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.can_attack(state) and self.get_kicks(state, 1)
                or self.has_slide(state),
            "Listless Library - Upper Back": lambda state:
                self.can_attack(state) and self.has_slide(state),
            "Listless Library - Locked Door Across": lambda state:
                self.has_slide(state),
            "Listless Library - Locked Door Left": lambda state:
                self.kick_or_plunge(state, 2)
                or self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Sansa Keep - Strikebreak": lambda state:
                self.has_breaker(state) and self.has_slide(state)
                or self.can_strikebreak(state) and self.has_plunge(state),
            "Sansa Keep - Lonely Throne": lambda state:
                self.has_breaker(state)
                and (
                    self.has_gem(state)
                    or self.can_bounce(state) and self.kick_or_plunge(state, 3)
                    or self.has_slide(state) and self.get_kicks(state, 3)),
            "Sansa Keep - Near Theatre": lambda state:
                self.has_slide(state),
            "The Underbelly - Rafters Near Keep": lambda state:
                self.has_slide(state),
            "The Underbelly - Main Room": lambda state:
                self.has_slide(state) or self.get_kicks(state, 1),
            "The Underbelly - Alcove Near Light": lambda state:
                self.get_kicks(state, 1) and self.has_slide(state),
            "The Underbelly - Building Near Little Guy": lambda state:
                self.get_kicks(state, 1)
                or self.has_slide(state),
            "The Underbelly - Strikebreak Wall": lambda state:
                self.can_strikebreak(state)
                and (
                    self.has_slide(state) and self.kick_or_plunge(state, 1)
                    or self.has_slide(state) and self.has_gem(state)),
            "The Underbelly - Surrounded By Holes": lambda state:
                self.can_soulcutter(state) and self.has_slide(state)
                or self.has_slide(state) and self.get_kicks(state, 1),
        }

        self.apply_clauses(region_clauses, location_clauses)

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
