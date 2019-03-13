# Project MMW
Free Source Game Development for Project MMW

Game Engine: Unreal Engine v4.21.1

Project MMW is a Creative Common game development project inspired by multiplayer pvp game such as mobile suit gundam online, SD Gundam Capsule Fighter (SDGO).

The game will be an online multiplayer pvp Mecha featuring 1v1 (duel) 2v2, 3v3, 5v5, 50v50 as well as a massive MMO eternal battleground experience (Think of GW2 mix with Dynasty Warriors). 

Features:
– Realtime PVP battle. 
– Custom Mecha with skill and equipment customisation/upgrades. 

Note: Because this is a free source game development project which everyone can contribute, features are subject to change during the development cycle.

## License
Please See license terms
By contribute to this project, you agree to the license terms of GNU Affero General Public License v3.0 with the sxception of special conditions outline below:
- Non-comercial right, this project is a free source project make by gamers for gamers and are strictly not allow for resale.

For any clarification, please contact repo admin.

## Contribution Guide:
Note: Please read and accept the license agreement before making any contributions.

Discord Channel: https://discord.gg/A2tnTSH

Source Code
GitHub: https://github.com/jinzwu/ProjectMMW.git

Game Assets
Google Drive Data File
https://drive.google.com/drive/folders/1IAeXF7u4frjrQvWNFAFoVIET_4PowFNP?usp=sharing

Dropbox: 
Public Read-Only
https://www.dropbox.com/sh/ayaopiheo87rup5/AABZPFfII4Lv2pC9fF1Jwk7ga?dl=0

Upload / File Request Link
https://www.dropbox.com/request/CCG3ezssLHY7aNjafcrX

Uploaded Temp Folder 
(Only use for if developer need new assets that was uploaded but haven’t been organised into the public folder)
https://www.dropbox.com/sh/ml5p23mhjunezyr/AACZw9vSwAmhtYAYXuk-CBLua?dl=0


Contribution instruction: 

FOR PROGRAMMERS:
1. Download the project from the GitHub repo above. (Recommend to use sourceTree)
2. Download the Latest game assets (LargeData) Folder from the Dropbox/Google Drive Public folder.
3. Add the LargeData folder to the Content Directory.
4. Make sure you are using the correct Unreal Engine version. 
5. Open Project in Unreal Engine. Make changes and test features.
6. Commit in a new branch, push and submit a pull request when feature is completed.
7. Contact Project Admin if there is any issues.

For Artists / Designers / Audio Engineers:
1. Check for existing appropriate files/assets in the Public folder for existing assets before contributing.
2. Upload appropriate files. preferably zipped. I will check and reorganised to the public folder if the upload files are suitable for the game.
3. Contact Project Admin if there are any issues. 

# Branching

## Quick Legend

<table>
  <thead>
    <tr>
      <th>Instance</th>
      <th>Branch</th>
      <th>Description, Instructions, Notes</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Stable</td>
      <td>stable</td>
      <td>Accepts merges from Working and Hotfixes</td>
    </tr>
    <tr>
      <td>Working</td>
      <td>master</td>
      <td>Accepts merges from Features/Issues and Hotfixes</td>
    </tr>
    <tr>
      <td>Features/Issues</td>
      <td>features-*</td>
      <td>Always branch off HEAD of master</td>
    </tr>
    <tr>
      <td>Hotfix</td>
      <td>hotfix-*</td>
      <td>Always branch off Stable</td>
    </tr>
  </tbody>
</table>

## Main Branches

The main repository will always hold two evergreen branches:

* `master`
* `stable`

The main branch should be considered `origin/master` and will be the main branch where the source code of `HEAD` always reflects a state with the latest delivered development changes for the next release. For Programmers, you will be branching and merging from `master`.

Consider `origin/stable` to always represent the latest code deployed to production. During day to day development, the `stable` branch will not be interacted with. For non-programmer, you will be pulling the project off stable.

When the source code in the `master` branch is stable and has been deployed, all of the changes will be merged into `stable` and tagged with a release number.

## Supporting Branches

Supporting branches are used to aid parallel development between team members, ease tracking of features, and to assist in quickly fixing live production problems. Unlike the main branches, these branches always have a limited life time, since they will be removed eventually.

The different types of branches we may use are:

* Feature branches
* Bug branches
* Hotfix branches

Each of these branches have a specific purpose and are bound to strict rules as to which branches may be their originating branch and which branches must be their merge targets. Each branch and its usage is explained below.

### Feature Branches

Feature branches are used when developing a new feature or enhancement which has the potential of a development lifespan longer than a single deployment. When starting development, the deployment in which this feature will be released may not be known. No matter when the feature branch will be finished, it will always be merged back into the master branch.

During the lifespan of the feature development, the lead should watch the `master` branch (network tool or branch tool in GitHub) to see if there have been commits since the feature was branched. Any and all changes to `master` should be merged into the feature before merging back to `master`; this can be done at various times during the project or at the end, but time to handle merge conflicts should be accounted for.

`<tbd number>` represents the project to which Project Management will be tracked.

* Must branch from: `master`
* Must merge back into: `master`
* Branch naming convention: `feature-<tbd number>`

#### Working with a feature branch

If the branch does not exist yet (check with the Lead), create the branch locally and then push to GitHub. A feature branch should always be 'publicly' available. That is, development should never exist in just one developer's local branch.

```
$ git checkout -b feature-id master                 // creates a local branch for the new feature
$ git push origin feature-id                        // makes the new feature remotely available
```

Periodically, changes made to `master` (if any) should be merged back into your feature branch.

```
$ git merge master                                  // merges changes from master into feature branch
```

When development on the feature is complete, the lead (or engineer in charge) should merge changes into `master` and then make sure the remote branch is deleted.

```
$ git checkout master                               // change to the master branch  
$ git merge --no-ff feature-id                      // makes sure to create a commit object during merge
$ git push origin master                            // push merge changes
$ git push origin :feature-id                       // deletes the remote branch
```

### Bug Branches

Bug branches differ from feature branches only semantically. Bug branches will be created when there is a bug on the live site that should be fixed and merged into the next deployment. For that reason, a bug branch typically will not last longer than one deployment cycle. Additionally, bug branches are used to explicitly track the difference between bug development and feature development. No matter when the bug branch will be finished, it will always be merged back into `master`.

Although likelihood will be less, during the lifespan of the bug development, the lead should watch the `master` branch (network tool or branch tool in GitHub) to see if there have been commits since the bug was branched. Any and all changes to `master` should be merged into the bug before merging back to `master`; this can be done at various times during the project or at the end, but time to handle merge conflicts should be accounted for.

`<tbd number>` represents the Basecamp project to which Project Management will be tracked. 

* Must branch from: `master`
* Must merge back into: `master`
* Branch naming convention: `bug-<tbd number>`

#### Working with a bug branch

If the branch does not exist yet (check with the Lead), create the branch locally and then push to GitHub. A bug branch should always be 'publicly' available. That is, development should never exist in just one developer's local branch.

```
$ git checkout -b bug-id master                     // creates a local branch for the new bug
$ git push origin bug-id                            // makes the new bug remotely available
```

Periodically, changes made to `master` (if any) should be merged back into your bug branch.

```
$ git merge master                                  // merges changes from master into bug branch
```

When development on the bug is complete, [the Lead] should merge changes into `master` and then make sure the remote branch is deleted.

```
$ git checkout master                               // change to the master branch  
$ git merge --no-ff bug-id                          // makes sure to create a commit object during merge
$ git push origin master                            // push merge changes
$ git push origin :bug-id                           // deletes the remote branch
```

### Hotfix Branches

A hotfix branch comes from the need to act immediately upon an undesired state of a live production version. Additionally, because of the urgency, a hotfix is not required to be be pushed during a scheduled deployment. Due to these requirements, a hotfix branch is always branched from a tagged `stable` branch. This is done for two reasons:

* Development on the `master` branch can continue while the hotfix is being addressed.
* A tagged `stable` branch still represents what is in production. At the point in time where a hotfix is needed, there could have been multiple commits to `master` which would then no longer represent production.

`<tbd number>` represents the Basecamp project to which Project Management will be tracked. 

* Must branch from: tagged `stable`
* Must merge back into: `master` and `stable`
* Branch naming convention: `hotfix-<tbd number>`

#### Working with a hotfix branch

If the branch does not exist yet (check with the Lead), create the branch locally and then push to GitHub. A hotfix branch should always be 'publicly' available. That is, development should never exist in just one developer's local branch.

```
$ git checkout -b hotfix-id stable                  // creates a local branch for the new hotfix
$ git push origin hotfix-id                         // makes the new hotfix remotely available
```

When development on the hotfix is complete, [the Lead] should merge changes into `stable` and then update the tag.

```
$ git checkout stable                               // change to the stable branch
$ git merge --no-ff hotfix-id                       // forces creation of commit object during merge
$ git tag -a <tag>                                  // tags the fix
$ git push origin stable --tags                     // push tag changes
```

Merge changes into `master` so not to lose the hotfix and then delete the remote hotfix branch.

```
$ git checkout master                               // change to the master branch
$ git merge --no-ff hotfix-id                       // forces creation of commit object during merge
$ git push origin master                            // push merge changes
$ git push origin :hotfix-id                        // deletes the remote branch
```

## Workflow Diagram

![Git Branching Model](http://f.cl.ly/items/3i1Z3n1T1k392r1A3Q0m/gitflow-model.001.png)  
*[gitflow-model.src.key](http://cl.ly/3V1b0c2F1H4024173S1M)*
