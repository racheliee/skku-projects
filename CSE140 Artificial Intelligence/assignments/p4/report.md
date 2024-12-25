---
title: CSE140 Project 4 Report
date: \today
geometry: margin=1in
papersize: letter
fontfamily: newtxtext, newtxmath
fontsize: 10pt
numbersections: true
autoEqnLabels: true
header-includes: |
  \usepackage{authblk}
  \author{Team Boo}
  \affil{Dept. of Systems Management Engineering\\University of California, Santa Cruz}
  \usepackage{fancyhdr}
  \pagestyle{fancy}
  \fancyhead[L]{CSE 140 P4 Report}
  \fancyhead[C]{}
  \fancyhead[R]{\thepage}
  \fancyfoot{}
  \usepackage{float}
  \let\origfigure\figure
  \let\endorigfigure\endfigure
  \renewenvironment{figure}[1][2] {\expandafter\origfigure\expandafter[H]} {
    \endorigfigure
  }
#   \usepackage[hangul, nonfrench, finemath]{kotex}
abstract: | 
  In this Pacman Capture the Flag project, we developed a team of two agents—one offensive and one defensive—to compete in a multi-agent, adversarial setting. Our offensive agent prioritizes efficient food collection and strategic evasion of enemy ghosts, guided by heuristic features and weighted evaluations. The defensive agent focuses on patrolling home territory, intercepting intruders, and protecting vital resources. Through iterative tuning of features and weights, our final solution achieves a balanced approach, improving performance over baseline strategies and adapting to various map layouts. This experience emphasizes the importance of feature engineering, heuristic design, and the interplay between offense and defense in complex, adversarial domains.
---

\tableofcontents

\newpage

# Team Boo: Pacman Capture the Flag Report

- Andrew Nguyen
- Carlo Aqui
- Kiaran Lau
- Rachel (Younwoo) Park

# Introduction

Our goal was to create a pair of Pacman Capture the Flag agents: one focusing on offense (aggressively collecting food) and the other focusing on defense (protecting our side from enemy invaders). This final project challenged us to balance multiple competing objectives, including navigating through partially observable territories, avoiding enemies as Pacman, and capturing enemy food. Simultaneously, we needed to guard against opposing agents trying to invade our side. Our approach incrementally evolved from a baseline strategy to a more heuristic-driven method guided by features and weights, aligning closely with adversarial search and reinforcement learning principles without explicitly implementing complex RL algorithms.

# Problem Modeling and Representations

The core objective is to increase our team's score by eating the opponent's food while preventing them from eating ours. Key challenges include:

- **Offense:** Efficiently navigating into enemy territory to gather food and capsules, and returning safely to home territory.
- **Defense:** Detecting and deterring invaders, staying close enough to intercept them, and minimizing the loss of our team's food.

**State Representation:**  
We represent state information through the game’s provided API (`CaptureGameState`), focusing on:

- **Agent Positions:** Tracks the current and potential future positions of both our agents and opponents.
- **Foods and Capsules:** Positions of food we need to eat and those we must protect, as well as power capsules that grant invulnerability or ghost-scaring abilities.
- **Territory Boundaries:** The midpoint of the layout, which delineates home turf from enemy territory, affecting whether our agent acts as a Pacman or a ghost.
- **Scared Timers and Invaders:** The state of enemies (scared or active), and whether they are invading our side.

**Agent Roles:**

1. **Offense Agent:** Focused on navigating towards and collecting enemy food and capsules. When crossing into enemy territory, it becomes a Pacman and must carefully avoid or exploit ghosts.
2. **Defense Agent:** Remains mostly on our side, positioning itself strategically to thwart enemy Pacmen. Its role is to guard critical food and capsules.

# Computational Strategy and Algorithms

## Offensive Agent Strategy

The offensive agent uses a feature-based evaluation function to guide its behavior. At each step:

1. **Feature Extraction:** We compute a set of features for each successor state:
   - **Food-Related:** Distance to nearest food, how much food is eaten compared to the previous state.
   - **Ghost-Related:** Minimum distance to active (dangerous) ghosts, presence of scared ghosts that can be eaten, and distance to capsules that can turn ghosts scared.
   - **Home Distance:** Incentive to return home if carrying a large amount of food or when danger is imminent.
   - **Crossover Behavior:** Encouragement or discouragement to cross the boundary, depending on the agent’s role.

2. **Weighted Linear Combination:** We define a set of weights for these features. The chosen action is the one maximizing the weighted sum of these features:
   ```python
   evaluation = sum(features[feature] * weights.get(feature, 0) for feature in features)
   ```
   
   We heavily reward eating food and staying safe from ghosts, while also incentivizing efficient movement towards capsules and strategic re-entry into home territory.

## Defensive Agent Strategy

The defensive agent also relies on a feature-based evaluation method, focusing on:

- **Number of Invaders:** How many enemy Pacmen are currently on our side.
- **Invader Distance:** Closeness to enemy invaders, favoring moves that reduce this distance.
- **Home Food and Capsule Defense:** Average distance to our defensive food and capsules, promoting a patrol-like behavior.
- **Patrolling Home Territory:** Encourages the agent to occupy strategic defensive positions near the boundary or high-value spots to ambush incoming opponents.

By aggregating these features, the defensive agent selects actions that minimize threats and attempt to intercept invaders efficiently. The weights penalize the presence and proximity of invaders, encouraging the agent to hold a strong defensive line.

# Obstacles and Challenges

Throughout development, we encountered several difficulties:

1. **Balancing Offense and Defense:** Achieving synergy between the offensive and defensive agents. Overly aggressive offense can risk being frequently captured, while passive defense may forfeit food opportunities.
2. **Partial Observability and Opponent Modeling:** Lack of perfect information made it challenging to predict the exact positions of invisible enemy agents. Initial attempts included simple heuristic assumptions (such as returning to patrol positions or revisiting important choke points).
3. **Computation Time and Efficiency:** We needed to ensure each decision was computed efficiently within the given time constraints. Our solution avoided complex searches and relied on heuristic evaluation for scalability.

# Evaluation of Our Agent

## Baseline Comparison

- **Dummy Agents:** Our agents outperformed the dummy agents by effectively capturing food and occasionally defending successfully. The offensive agent quickly outpaced the dummy in scoring, while the defensive agent was able to at least partially intercept dummy Pacmen.

- **Baseline Team (Reflex Agents):** While initially comparable, our agents improved upon the baseline by incorporating additional features like capsule distances and better defense positioning. Our offensive agent also benefited from a more nuanced scoring system that prioritized food, capsules, and survival.

## Internal Testing and Simulations

We tested various layouts, including:

- **Default Layout:** Agents learned to navigate the corridors efficiently, avoiding ghosts and securing food.
- **Randomized Layouts:** Reinforced the need for general strategies. Our heuristic-driven approach, focusing on distances and counts, proved flexible.

Our final performance improved over time as we tuned weights:

- Higher penalties for being near active ghosts improved survival rates.
- Increased emphasis on capsule distances enabled more strategic use of power pellets.
- Adjustments to defensive patrol points and invader distance weights led to more effective interceptions.

# Lessons Learned

1. **Heuristic Design Matters:** Fine-tuning feature weights was critical. Small changes to weights drastically affected behavior.
2. **Collaboration and Code Integration:** Splitting offense and defense allowed clearer reasoning and simpler debugging. However, ensuring both agents’ strategies complemented each other was a non-trivial task.
3. **Trade-Offs in Complexity:** More complex modeling (like probability distributions for unseen enemies) might yield better results but was difficult within time and computational constraints.

# Conclusion

Our final solution integrated heuristic-driven offensive and defensive agents to tackle the Pacman Capture the Flag challenge. The offensive agent prioritizes grabbing food and escaping safely, guided by strategic capsule usage. The defensive agent maintains vigilance over our territory, intercepting intruders and protecting crucial resources.

While our solution may not dominate all opposing teams, it represents a well-structured, modular approach balancing complexity and performance. Through this project, we gained practical experience with adversarial reasoning, heuristic design, and feature engineering in a multi-agent, partially observable environment.