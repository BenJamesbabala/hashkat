[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Random Follow Model

A discussion on follow models and an exercise using the random follow model, this tutorial should take approximately 30 minutes to complete.

There are six different follow models that have been implemented into #k@, to allow greater flexibility and control in the networks the project simulates.
The six follow models are:

* Random
* Twitter Suggest
* Entity
* Preferential Entity
* Hashtag
* Twitter

Each of these follow models will be discussed and analyzed in each of their own tutorial, starting with this one, where we study the random follow model.
The input data used to create the networks will be relatively the same for each follow model tutorial, so that the results from running each different follow model
can be easily compared and constrasted.

As the name implies, a random follow Mmdel is one in which the entities in which a twitter user follows is totally arbitrary.
You've already had some experience working with the random follow model, the simple network we created in Tutorial 1 implemented it,
but in this tutorial we are going to go into this model in much more detail. We are going to actually go through the input file and make some changes
to create a simulation of our desired framework.

For reference, all the files that we will use in this simulation can be found in the tutiorals directory in hashkat, under the title *tutorial03*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/tree/master/tutorials/tutorial03/INFILE.yaml).

Let's go into the INFILE.yaml file in the hashkat directory and make some changes. Using the INFILE.yaml used in Tutorial 1 as a starting point,
we'll keep the number of entities within the
simulation constant by having 1000 initial and maximum entities. For quickness, we'll lower the maximum simulated time to 1000 simulated minutes.
We'll keep the maximum analysis steps to be unlimited for the most refined simulation, and also keep the max real time to 1 minute since we don't 
want the simulation to run for any longer than that. Interactive mode shall remain off, so both *enable_interactive_mode* and
*enable_lua_hooks* shall be *false* and the lua script will be irrelevant.*use_barabasi* we'll be used next tutorial,
so this shall be kept *false* and the Barabasi connections will therefore also be inconsequential.
The Barabasi exponent will remain at 1 since this gives the most realistic results. *use_random_time_increment* will be set to true
for superior results. *use_followback* will remain *false*, and the follow model will remain *random*, with the model weights,
which are only necessary for the twitter follow model, being ignored. We set them all to 0 for this example just for clarity,
but they will have no impact on this simulation either way because we are running a random, not a twitter, follow model.
We're going to not allow unfollowing to occur in this example,
but you are encouraged to experiment with it when creating your own network simulations, so we're going to set *stage1_unfollow* to false
and the unfollow tweet rate to an exceptionally high number for this simulation like 10,000 tweets.
We have also kept the probability of hashtag presence in tweets to be 50%.

Since the number of entities within this simulation will remain constant, the add rate will therefore be 0. The output section of INFILE.yaml
will also be left as is, to produce as many data files for analysis as possible. The tweet and retweets ranks have remained the same,
though to be honest their purpose is just to organize them and has no real impact on the simulation. The follow ranks max threshold
and weight have also remained the same, with their minimum values being 0 and 1 and their maximum values being a number greater than the number of entities
and that value increased by one. Though this will not impact this simulation, it is very important for the twitter suggest follow model, and will therefore be
elaborated on in the next tutorial. The *tweet_observations*
section of this program will be kept as is for all of these tutorials, though you are encouraged to change it and experiment with it
when running your own simulations. More information on this can be found on the Input page.

The ideologies have remained the same, but the *regions* sections has changed slightly. Instead of having two regions, with each region speaking
a different language, we have removed Quebec from this simulation by reducing its *add_weight* to zero. For simplicity, we're going to focus
on just one region speaking only one language for these follow model tutorials. We will also continue to use the *NoRetweet* preference class,
again, to to keep these simulaitons as straightforward as possible.

We have also add another entity type to our input file. This *Celebrity* entity type is of course intended to mimic a Celebrity user, while a
*Standard* user is used to portray a typical entity on a social network. In contrast to the *Standard* users from **Tutorial 1**, the *Standard* entities in this
simulation will have a *followback_probability* of 0, so that entities will only follow other entities through the follow model,
and a constant follow rate of 0.01 instead of 0.0001, to allow for more following to occur. The *Celebrity* user will have all of the same input values as the
*Standard* user, except that its add weight will be zero. Though we have included them in the input file, the *Celebrity* users will not be used until a later
tutorial, where entities will follow other entities based on their entity type.  

Running this simulation, and analyzing our output, we can see that most
entities have a cumulative-degree distribution of 20, as expected, and we
can see that we have a much more complicated network of entities, with the purple nodes corresponding to the abundant Standard users, and
the different coloured nodes corresponding to the Celebrity, Bot, and Organization type entities, which are all roughly the same quantity.
These two separate networks of entities generated from our simulation again correspond to our English-speaking and French-speaking entities,
with the much more denser network representing the greater number and association of English-speaking entities.

![Cumulative Degree Distribution](/img/tutorial04/cumulative-degree_distribution_month_000.svg =1x  "Cumulative Degree Distribution")

![Visualization](/img/tutorial04/visualization.png =1x  "Visualization")

## Next Steps

With the completion of this tutorial, we have worked with the simplest of the six types of follow models.
Though we went through it in great detail, this example was just the tip of the iceberg of all the random follow model simulations you can
create. Try running your own random follow model simulation different from the one outlined above, and see what you can create.

When ready, move on to the next tutorial, where things get a lot more interesting and a little more complicated with the
twitter suggest follow model.