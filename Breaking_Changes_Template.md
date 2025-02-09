---
title: Replace with title of breaking change
description: >
  Brief description similar to the "context" section below.
  Text should break at 80 chars or less.
---

<!--
PLEASE READ THESE GENERAL INSTRUCTIONS:

* All lines of text should be 80 chars OR LESS.
  The writers strongly prefer semantic line breaks.
* DON'T SUBMIT a PR weeks and weeks in advance.
  Ideally, submit a PR once you have confirmed
  info on the version number where the breaking
  change landed.
* One of the most important things to fill out
  in this template is the *Timeline* section.
  I won't approve/merge the PR until the "landed in"
  release info is provided. For example:
  `Landed in version: 1.21.0-5.0.pre`.
  Do NOT list the PR in this section.
* The text in this page should be backwards looking,
  so write about previous behavior in past tense,
  not future tense. People are reading this months
  from now when the change is likely in the stable
  release, not today. Don't say "in a month" or
  talk about your plan to do something next week.
  Assume you've done it, and that they're looking
  back to figure out how to migrate their code.
* Use sentence case for headings and titles.
  (`## Migration guide`, NOT `Migration Guide`)
* DON'T use the abbreviation `i.e.` or `e.g.`.
  Use "for example" or "such as", and similar.
* For links, use the macros where possible.
  But don't use "github.com" in your URLs.
  Use the {{site.github}}, macros.
* AVOID "will" when possible, in other words,
  stay in the present tense. For example:
  Bad: "When encountering an xxx value,
        the code will throw an exception."
  Good: "When encountering an xxx value,
          the code throws an exception."
  Good use of "will": "In release 2.0, the xxx API
        will be deprecated."
* Finally, delete the comment tags and text from the
    final PR.
-->

## Summary

<!--
A brief (one- to three-line) summary that gives
context as to what changed so that someone can
find it when browsing through an index of
breaking changes, ideally using keywords from
the symptoms you would see if you had not yet
migrated (for example, the text from probable
        error messages).
-->

## Background

<!--
High-level description of what API changed and why.
Should be clear enough to be understandable to someone
who has no context about this breaking change,
such as someone who doesn't know the underlying API.
This section should also answer the question
"what is the problem that led to considering making
a breaking change?"

Include a technical description of the actual change,
with code samples showing how the API changed.

Include examples of the error messages that are produced
in code that has not been migrated. This helps the search
engine find the migration guide when people search for those
error messages. THIS IS VERY IMPORTANT FOR DISCOVERY!
-->

## Migration guide

<!--
A description of how to make the change.
If a migration tool is available,
discuss it here. Even if there is a tool,
a description of how to make the change manually
must be provided. This section needs before and
after code examples that are relevant to the
developer.
-->

Code before migration:

```c
// Example of code before the change.
```

Code after migration:

```c
// Example of code after the change.
```

## Timeline

<!--
The version # where this change was
introduced. If there is a deprecation window,
the version # to which we guarantee to maintain
the old API. Use the following template:

Landed in version: xxx
In stable release: not yet
Reverted in version: xxx  (OPTIONAL, delete if not used)
-->

## References

<!--
  These links are commented out because they
  cause the GitHubActions (GHA) linkcheck to fail.
  Remove the comment tags once you fill this in with
  real links. Prefer our stable documentation
  if possible.

API documentation:

* [`ClassName`][]

Relevant issues:

* [Issue xxxx][]
* [Issue yyyy][]

Relevant PRs:

* [PR title #1][]
* [PR title #2][]
-->

<!--
  Add the links to the end of the file in alphabetical order.
  The following links are commented out because they make
  the GitHubActions (GHA) link checker believe they are broken links,
  but please remove the comment tags before you commit!

  If you are sharing new API that hasn't landed in
  the stable channel yet, use the master channel link.
  To link to docs on the master channel,
  include the following note and make sure that
  the URL includes the master link.
-->

<!-- Stable channel link: -->
<!--
[`ClassName`]: [link_to_relevant_page].html
-->

<!-- Master channel link: -->
<!--
[`ClassName`]: [link_to_relevant_page].html
-->

<!--
[Issue xxxx]: [link_to_actual_issue]
[Issue yyyy]: [link_to_actual_issue]
[PR title #1]: [link_to_actual_pr]
[PR title #2]: [link_to_actual_pr]
-->
