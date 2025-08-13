from test.bases import WorldTestBase

from .. import PseudoregaliaWorld


class PseudoTestBase(WorldTestBase):
    game = "Pseudoregalia"
    world: PseudoregaliaWorld


class PseudoKeyHintsBase(PseudoTestBase):
    run_default_tests = False
    expect_hints: bool
    major_key_names = [
        "Major Key - Empty Bailey",
        "Major Key - The Underbelly",
        "Major Key - Tower Remains",
        "Major Key - Sansa Keep",
        "Major Key - Twilight Theatre",
    ]

    def test_key_hints(self):
        unfilled = [location for location in self.multiworld.get_unfilled_locations(self.player)
                    if location.address and not location.item]
        assert len(unfilled) >= 5, "Not enough locations to test keys"

        locations = unfilled[:5]
        for i in range(5):
            locations[i].item = self.world.create_item(self.major_key_names[i])
        slot_data = self.world.fill_slot_data()
        for location in locations:
            location.item = None

        if self.expect_hints:
            assert "key_hints" in slot_data, "Expected key_hints in slot_data"
            expected_key_hints = [
                [{
                    "player": self.player,
                    "location": location.address,
                }]
                for location in locations
            ]
            assert slot_data["key_hints"] == expected_key_hints, \
                f"Expected {expected_key_hints} but found {slot_data["key_hints"]}"
        else:
            assert "key_hints" not in slot_data, "Expected no key_hints in slot_data"
