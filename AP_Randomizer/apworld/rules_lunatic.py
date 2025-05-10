from .rules_expert import PseudoregaliaExpertRules


class PseudoregaliaLunaticRules(PseudoregaliaExpertRules):
    def __init__(self, world) -> None:
        super().__init__(world)

        region_clauses = {
            "Tower Remains -> The Great Door": lambda state:
                self.has_slide(state) and self.get_kicks(state, 1),  # possible with plunge instead?
            "Bailey Lower -> Bailey Upper": lambda state:
                self.can_bounce(state),
            "Theatre Pillar -> Theatre Main": lambda state:
                self.get_kicks(state, 2),  # bubble route
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle Main -> Castle => Theatre Pillar": lambda state:
                self.has_plunge(state),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.get_kicks(state, 3),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_slide(state) and self.kick_or_plunge(state, 2),
            "Library Main -> Library Top": lambda state:
                self.get_kicks(state, 1),
            "Library Top -> Library Greaves": lambda state:
                self.can_bounce(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Underbelly Main Lower -> Underbelly Main Upper": lambda state:
                self.has_slide(state)
                and (
                    self.has_gem(state)
                    or self.get_kicks(state, 2)
                    or self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.get_kicks(state, 1) and self.has_breaker(state)),
        }

        location_clauses = {
            # "Twilight Theatre - Center Stage": lambda state:
            #     TODO: theoretical logic for soulcutterless or gemless
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_slide(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.can_bounce(state) and self.kick_or_plunge(state, 1)
                or self.has_slide(state),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_slide(state) and self.kick_or_plunge(state, 1),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.can_bounce(state),
            "Castle Sansa - Corner Corridor": lambda state:
                self.get_kicks(state, 1) and self.has_slide(state),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.kick_or_plunge(state, 1),  # This never really matters and that makes me sad
            "Sansa Keep - Levers Room": lambda state: True,
            "Sansa Keep - Lonely Throne": lambda state:
                self.has_breaker(state) and self.has_slide(state) and self.kick_or_plunge(state, 3)
                or (
                    self.has_slide(state)
                    and self.can_bounce(state)
                    and self.get_kicks(state, 1)
                    and self.has_plunge(state)
                    and self.can_soulcutter(state)),
            "Listless Library - Upper Back": lambda state:
                self.has_plunge(state),
        }

        self.apply_clauses(region_clauses, location_clauses)

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
