SELECT movies.title FROM movies GROUP BY movies.title
HAVING movies.id IN (SELECT stars.movie_id FROM stars GROUP BY stars.movie_id
HAVING stars.person_id IN (SELECT people.id FROM people
WHERE people.name IN ("Johnny Depp", "Helena Bonham Carter")));
