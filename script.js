/* let's us animate the custom prop */
/* if @property isn't supported, everything works, just without the animation */
@property --progress {
    syntax: "<length-percentage>";
    inherits: false;
    initial-value: 0%;
  }
  
  [role="progressbar"] {
    --size: 132px;
    --bar-width: 20px;
  
    font-size: 2rem;
    width: var(--size);
    aspect-ratio: 1 / 1;
    margin: 2rem auto;
    background: conic-gradient(
      var(--color-dark),
      var(--color-dark) var(--progress),
      var(--color-dark-glare) 0%
    );
    border-radius: 50vmax;
  
    display: grid;
    place-items: center;
  
    /* this works thanks to the @property at the top */
    transition: --progress 500ms linear;
  
    /* we only have this role if JS is enabled */
    /* which means it's safe to hide the message here */
    > span {
      display: none;
    }
  }
  
  /* using aria-valuenow, to help enforce using it in the JS */
  [role="progressbar"]::after {
    content: attr(aria-valuenow) "%";
    background: white;
    width: calc(100% - var(--bar-width));
    aspect-ratio: 1;
    border-radius: inherit;
    display: grid;
    place-items: center;
  }
  
  /* when progress === 100 */
  [role="progressbar"][aria-valuenow="100"]::after {
    /* this is kind of hacky, but it allows the percent to be at 100% for a bit before the checkbox comes in, which feels better */
    animation: progressComplete 0s forwards;
    animation-delay: 1000ms;
  }
  
  @keyframes progressComplete {
    to {
      content: url("data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iODUiIGhlaWdodD0iODUiIHZpZXdCb3g9IjUgMzAgNzUgMTIiIGZpbGw9Im5vbmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CjxwYXRoIGQ9Ik0zNS40MjM3IDUzLjczMjdMNjcuOTc4NyAyMS4xNzc3TDcyLjk4OTUgMjYuMTg0MkwzNS40MTk1IDYzLjc1TDEyLjg4NiA0MS4yMTIyTDE3Ljg5MjUgMzYuMjAxNUwzNS40MjM3IDUzLjczMjdaIiBmaWxsPSIjMWYxYTM4Ii8+Cjwvc3ZnPgo=");
      background: var(--color-success);
    }
  }
  
  /* if js is disabled */
  .progressbar:not([role="progressbar"]) {
    position: relative;
    &::after {
      display: none;
    }
    span {
      font-size: 1rem;
      text-align: center;
      color: var(--color-success-dark);
    }
  }
  
  @layer general-styling-and-whatnot {
    :root {
      --font-base: "Space Mono", monospace;
      --color-dark: #1f1a38;
      --color-dark-glare: #989ea9;
      --color-success: #76f7bf;
      --color-success-dark: #2d7a59;
    }
  
    * {
      box-sizing: border-box;
    }
  
    body {
      font-family: var(--font-base);
      color: var(--color-dark);
      width: min(100% - 3rem, 40ch);
      margin-inline: auto;
      margin-block: 3rem;
    }
  
    a {
      color: var(--color-dark);
  
      &:is(:hover, :focus) {
        color: var(--color-success-dark);
      }
    }
  
    .testing-ground {
      color: var(--color-success);
      font-weight: bold;
      background: var(--color-dark);
      border: 5px solid var(--color-dark-glare);
      padding: 1rem;
      margin-block: 3rem;
  
      display: grid;
      gap: 1rem;
    }
  
    .testing-ground button {
      cursor: pointer;
      font: inherit;
      background: white;
      color: var(--color-dark);
      border: 0;
      padding: 0.5rem;
  
      &:is(:hover, :focus-visible) {
        background: var(--color-success);
      }
  
      &:is(:focus-visible) {
        outline: 5px solid var(--color-dark-glare);
      }
    }
  }
  