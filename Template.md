# PUBLICLY SHARED

1. File an issue using the [design doc issue template](https://github.com/lurkydismal/qr/issues/new?labels=design+doc&template=8_design_doc.yml) and assign it to yourself.

1. If you want to solicit feedback on your design doc, see [suggestions for how to do this](Design_Documents.md#soliciting-feedback).

Not every section applies to every design doc.

## SUMMARY

Place a _very_ short introduction to your discussion or design here.

__Author: {Author Name} ({GitHub ID})__

__Created:__ {Month}/{Year} / __Last updated:__ {Month}/{Year}

## WHAT PROBLEM IS THIS SOLVING?

Objectives of the discussion or design.

_Suggestion_: If you’re proposing a user-facing change to qr, describe the problem this proposal addresses from the user’s point of view, including:

* Who are the intended users? Be as specific as possible, in terms of their roles, responsibilities, levels of expertise, and other attributes that might have an impact on how likely they’d find your solution useful and usable.

* What is the problem this solution can help the intended users solve?

Here are some examples:

_In progress_.

## BACKGROUND

Background needed to understand the problem domain. Don’t include any _solutions_ in this section.

### Audience

Consider who you are writing for. Be explicit about who your audience is. Anyone contributing to qr? Just people who are regular contributors to a particular source file? Only you and your TL? The background section should be sufficient for people in your audience to understand your proposal.

### Glossary

* __Terms Relevant to Discussion__ - A minimal set of definitions of terms needed to understand the problem domain go here. Do not include widely known concepts (e.g. don’t define "URL" or "GUI"), just things needed to understand the discussion.

## OVERVIEW

Overview of the design or discussion.

### Non-goals

What is specifically not being addressed by this discussion or design.

## USAGE EXAMPLES

How would the user experience the proposed solution? Provide a few examples illustrating how the user will use the proposed solution in realistic scenarios. If the proposal is an API, include sample code.

Include brief inline explanations of key concepts this proposal introduces. If
applicable, show before-and-after comparisons.

If the proposal is a GUI or CLI, include a detailed description of each usage scenario.

You’re also encouraged to include [paper sketches](https://www.smashingmagazine.com/2021/09/power-pen-paper-sketching/), [wireframes](https://www.usability.gov/how-to-and-tools/methods/wireframing.html#:~:text=A%20wireframe%20is%20a%20two,styling%2C%20color%2C%20or%20graphics.) or [storyboards as visual aid](https://www.nngroup.com/articles/storyboards-visualize-ideas/).It’s completely fine to keep them at a low fidelity (e.g., paper-based) to facilitate discussions.

You may integrate usage examples into the Detailed Design/Discussion section if doing so makes your doc flow better. However, mark them clearly with subsection headings, so a reader who doesn’t wish to review the implementation can still provide feedback on the intended usage of your proposed solution.

## DETAILED DESIGN/DISCUSSION

Detailed Design. Discuss.

## ACCESSIBILITY

Explain how accessibility needs will be addressed by the proposal.

## INTERNATIONALIZATION

Explain how internationalization and localization needs will be addressed by the proposal.

## INTEGRATION WITH EXISTING FEATURES

Explain how existing features are affected by the proposal. How will you adjust existing APIs and features so that in the end, the combined API feels like it was always designed to have the proposed feature?

## OPEN QUESTIONS

* Will it work?

## TESTING PLAN

Provide a description of testing or a link to a testing plan here, if the discussion involves something that can be tested.

## DOCUMENTATION PLAN

Provide a description of the planned documentation and code samples that will be provided for the implementation, both for the API documentation and for any specialized articles for the website.

## MIGRATION PLAN

Provide a description of the migration plan or a link to a migration plan here, if the discussion involves something that must be migrated.
