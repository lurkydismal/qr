Contributing to qr
=======================

_tl;dr: be [courteous](CODE_OF_CONDUCT.md), follow the steps below to set up a development environment;_

Welcome
-------

We invite you to join the qr team, which is made up of volunteers!
There are many ways to contribute, including writing code, filing issues on GitHub, helping people
on our chat channels, or helping to triage, reproduce, or
fix bugs that people have filed, adding to our documentation,
doing outreach about qr, or helping out in any other way.

We grant commit access (which includes full rights to the issue
database, such as being able to edit labels) to people who have gained
our trust and demonstrated a commitment to qr.

We communicate primarily over GitHub.

Before you get started, we encourage you to read these documents which describe some of our community norms:

1. [Our code of conduct](CODE_OF_CONDUCT.md), which stipulates explicitly
    that everyone must be gracious, respectful, and professional. This
    also documents our conflict resolution policy and encourages people
    to ask questions.

2. [Values](Values.md)

Helping out in the issue database
---------------------------------

Triage is the process of going through bug reports and determining if they are valid, finding out
how to reproduce them, catching duplicate reports, and generally making our issues list
useful for our engineers.

If you want to help us triage, you are very welcome to do so!

1. Read [our code of conduct](CODE_OF_CONDUCT.md), which stipulates explicitly
    that everyone must be gracious, respectful, and professional. If you're helping out
    with triage, you are representing the qr team, and so you want to make sure to
    make a good impression!

1. You won't be able to add labels at first, so instead start by trying to
    do the other steps, e.g. trying to reproduce the problem and asking for people to
    provide enough details that you can reproduce the problem, pointing out duplicates,
    and so on.

1.
    * `P4` means "important bug", and each level above `P4` (from `P3` to `P0`) is an order of magnitude higher importance.
    * If you have permission, assign bugs to yourself if you're working on them.
    * Unassign bugs that you are not working on soon.
    * If an issue is not assigned, assume it is available to be worked on.

Quality Assurance
-----------------

One of the most useful tasks, closely related to triage, is finding and filing bug reports. Testing
beta releases, looking for regressions, creating test cases, adding to our test suites, and
other work along these lines can really drive the quality of the product up. Creating tests
that increase our test coverage, writing tests for issues others have filed, all these tasks
are really valuable contributions to open source projects.

If this interests you, you can jump in and submit bug reports without needing anyone's permission!
We're especially eager for QA testing when we announce a beta release.

With each beta we need to test that there are no regressions. We have lots of automated tests, but sometimes the thing that breaks is something we hadn't thought to test (or haven't figured out how to test) and so human testing is important!

If you want to contribute test cases, you can also submit PRs. See the next section
for how to set up your development environment.

Developing for qr
----------------------

If you would prefer to write code, you may wish to start with our list of [good first contributions](https://github.com/lurkydismal/qr/issues?q=is%3Aopen+is%3Aissue+label%3A%22good+first+contribution%22).

To develop for qr, you will eventually need to become familiar
with our processes and conventions. This section lists the documents
that describe these methodologies. The following list is ordered: you
are strongly recommended to go through these documents in the order
presented.

1. [Tree hygiene](Tree_hygiene.md),
    which covers how to land a PR, how to do code review, how to
    handle breaking changes, how to handle regressions, and how to
    handle post-commit test failures.

2. [qr design doc template](Template.md),
    which should be used when proposing a new technical design. This is a good
    practice to do before coding more intricate changes.
    See also our [guidance for writing design docs](Design_Documents.md).

Releases
--------

If you are interested in participating in our release process, which may involve writing release notes and blog posts, coordinating the actual
generation of binaries, updating our release distribution, and other work of that nature, then please contact @lurkydismal .
