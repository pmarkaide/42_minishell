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

# Ensure src/main.c from the tester branch is kept
echo "Ensuring src/main.c remains unchanged in $TEST_BRANCH..."
git checkout --ours src/main.c

# Stage the resolved file
git add src/main.c

# Commit the merge with a message
git commit -m "Merged $PRODUCTION_BRANCH into $TEST_BRANCH, keeping the test branch's src/main.c"

echo "Update complete. Test branch is now up-to-date with the production branch."