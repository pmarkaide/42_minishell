#!/bin/bash

# Define the branches
PRODUCTION_BRANCH="main"
TEST_BRANCH="tester"

echo "Switching to $TEST_BRANCH branch..."
git checkout $TEST_BRANCH || { echo "Failed to checkout $TEST_BRANCH. Exiting."; exit 1; }

# Fetch the latest changes from the remote repository
echo "Fetching latest changes..."
git fetch origin

# Merge the production branch into the test branch
echo "Merging $PRODUCTION_BRANCH into $TEST_BRANCH..."
git merge origin/$PRODUCTION_BRANCH

# Check for merge conflicts
if git diff --name-only --diff-filter=U | grep -q "^main.c$"; then
  echo "Conflict detected in main.c. Resolving conflict by keeping the test branch's version..."

  # Checkout the test branch's version of main.c
  git checkout --ours main.c

  # Stage the resolved file
  git add main.c

  # Commit the merge with a message
  git commit -m "Merged $PRODUCTION_BRANCH into $TEST_BRANCH, keeping the test branch's main.c"
else
  echo "Merge completed without conflicts."
fi

echo "Update complete. Test branch is now up-to-date with the production branch."
