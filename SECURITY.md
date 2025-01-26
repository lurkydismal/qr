# Security Policy

## Supported Versions

We commit to publishing security updates for the version of qr currently
on the `stable` branch.

## Expectations

We treat security reports equivalent to a P0 priority level. This means that we attempt to fix them as quickly as possible.
We will release a beta or hotfix for any major security report found in the most recent stable version of our program.

## Reporting a Vulnerability

We use GitHub's security advisory feature to track open security reports. You should expect
a close collaboration as we work to resolve the security vulnerability you have reported. Please reach out again
if you do not receive prompt attention and regular updates.

## Flagging Existing Issues as Security-related

If you believe that an existing github issue is security-related, we ask that you write a comment.
The comment should include a short description of why it should be handled according to this security policy.

Security reports are not tracked explicitly in the github issue database.

## Disclosure Process

This section describes the process used by the qr team when handling vulnerability reports.

Vulnerability reports are received via the github comments under issue. Certain team members
who have been designated the "vulnerability management team" receive these notifications. When receiving
such an notification, one of the vulnerability management team members will:

1. Reply to the commetn acknowledging its receipt, cc'ing other
members of the team are aware that they are handling the security report. If the comment does not describe
an actual vulnerability, the process will stop here. (Unfortunately, we do receive spam, and well-meaning but ultimately misguided reports that do not represent issues for which this process is appropriate, at this address.)
2. Triage the report to evaluate its impact and if it is a security vulnerability.
3. Collaborate with the appropriate team lead to ensure that an owner is assigned to the report.
The owner will drive it through the fix and release process.
4. Work with the team lead and product manager to determine if this security report requires a security advisory.
5. Create a new [security advisory](https://github.com/lurkydismal/qr/security/advisories/new) if an advisory is required.
One must be the repo admin to do this. Vulnerability management team members who are not also a repo admin will reach out to the repo admins until they find one who can create the advisory. The repo admins who are also vulnerability
management team members are @lurkydismal.
6. Reach out to the reporter to ask them if they would like to be involved and whether they would like to be credited.
For credit, the GitHub security advisory UI has a field that allows contributors to be credited.
7. Add the [vulnerability reporter](https://docs.github.com/en/free-pro-team@latest/github/managing-security-vulnerabilities/adding-a-collaborator-to-a-security-advisory), relevant team lead and fix owner to the security advisory so that they can get updates.
8. If the security issue does not yet have a CVE number, as a contributor, request one from go/cve-request. Every security advisory will have a CVE number.
9. Work with the release and PR team to coordinate the publication of the security advisory.

## Receiving security updates

We will announce security advisories in the technical release blog post.

qr does not have a bug bounty program.

## References

- For more information on security advisories, see
[the GitHub documentation](https://docs.github.com/en/free-pro-team@latest/github/managing-security-vulnerabilities/managing-security-vulnerabilities-in-your-project).

**If team members from other organizations would like their team's playbook listed here for their reference (even if it is not a public resource), please submit a PR.**
