from hashkat_util import TestBase, hashkat_test, hashkat_dump_state, hashkat_dump_summary,  hashkat_dump_stats

###############################################################################
## Hash tag retweet test
###############################################################################

# Degenerate network: Stagnant network. No rates at all.
class Stagnant_network_exits_correctly(TestBase):
    base_infile = "base_infiles/simple-base.yaml"
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = 2
        yaml["analysis"]["initial_agents"] = 0
        # Nullify all actions: 
        yaml["rates"]["add"] = {"function": "constant", "value": 0.0}
        for agent_type in yaml["agents"]: 
            for rate in agent_type["rates"]:
                agent_type["rates"][rate] = {"function": "constant", "value": 0}
        print "Note: A WARNING message is expected."
    def on_exit(self):
        # Test that a stagnant network does not result in creation events:
        assert hashkat_dump_stats(self.state)["n_agents"] == 0
        # Test that a stagnant network does not result in step events at all:
        assert hashkat_dump_stats(self.state)["n_steps"] == 0

hashkat_test(Stagnant_network_exits_correctly())

# Degenerate network: Pure growth. No additions or tweets.
class Pure_growth_network_grows_correctly(TestBase):
    base_infile = "base_infiles/simple-base.yaml"
    def __init__(self, AGENTS_TO_GROW):
        self.AGENTS_TO_GROW = AGENTS_TO_GROW
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.AGENTS_TO_GROW
        yaml["analysis"]["max_agents"] = self.AGENTS_TO_GROW
        yaml["analysis"]["initial_agents"] = 0
        yaml["rates"]["add"] = {"function": "constant", "value": 1.0}
        # Make user addition the only action:
        for agent_type in yaml["agents"]: 
            for rate in agent_type["rates"]:
                agent_type["rates"][rate] = {"function": "constant", "value": 0}
    def on_exit(self):
        stats = hashkat_dump_stats(self.state)
        # Test that a network with only addition rates grows 1 user per step: 
        assert stats["n_agents"] == self.AGENTS_TO_GROW

hashkat_test(Pure_growth_network_grows_correctly(AGENTS_TO_GROW = 1000))

