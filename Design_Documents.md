# Design documents

We have a [template](Template.md) you can use. It describes the process for your design doc.
We recommend you use that template so that people can immediately recognize that this is a qr design document and that it is shared publicly.

When you implement a design, document it in the source code in detail. The API documentation is the usual place where we document our designs. It's perfectly reasonable for API docs to be multiple pages long with subheadings. Do not assume that anyone will ever read your design doc after the discussion has finished. Similarly, do not assume that anyone will look at closed GitHub issues or PR discussions.

## Purpose of design docs

The qr project uses design docs as a tool for guiding discussions.

Decisions are made in PRs, not in design docs.

Approvals are given in PRs, not in design docs.

## Soliciting feedback

If you wish to get feedback on your design doc, you have many options for doing so, depending on how much feedback you want:

* If there is an issue already filed on the topic, definitely put a link to the design doc there. People who have found the issue and want to get updates on the topic will have subscribed to the issue, so this is the most effective way to communicate with them.

* You can ask our user experience researcher (UXR) team to study the proposal and potentially test it with real users, or collect relevant data from the next quarterly survey.

### How to get good feedback

Often, you will solicit feedback, and get none. There are many causes of this.

Maybe your proposal is unclear, and so people don't really know what to suggest. People are often reluctant to provide broad criticisms. Consider if you can improve the clarity of your design doc. Do you have a clear problem statement separate from your solution? Do you show example code of the problem? Do you have screenshots or diagrams of the problem? For your solution, do you start from first principles and explain it? Often it's easy to forget that your readers don't have the same context you do, so without a gentle introduction they'll get lost very quickly. Do you have sample code of your proposed solution(s)? Do you need more diagrams or screenshots? Ask someone you trust if they think your document is sufficiently clear.

Maybe your proposal is too big for anyone to get their head around. Can it be split into smaller components, so that each one can be understood separately, before bringing all the pieces together into your grand design? (You can do this all in the same doc.)

Maybe people don't know what to provide feedback about. If you have an area you are particularly interested in getting feedback about, it can be very helpful to explicitly invite such feedback.

Maybe you are asking the wrong people. Consider the suggestions in the earlier section, and reach out explicitly to people who are affected by your proposal. Consider escalating, asking more and more people until you get the volume of feedback you desire.

Maybe everyone agrees. Consider leaving some intentionally sketchy details in your proposal to encourage people to engage! (This is a risky strategy, sometimes people end up _liking_ your "bad" ideas...)

Maybe your proposal is too obvious or uninteresting. Sometimes, a change is so uncontroversial and simple that frankly it would be better just to write the PR and submit it.

## Content in design docs

### Diagrams

As we use Markdown for the text portion of design docs, the easiest way to draw diagrams is using [Mermaid](https://github.blog/2022-02-14-include-diagrams-markdown-files-mermaid) Diagrams.
