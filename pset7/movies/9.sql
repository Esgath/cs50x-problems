SELECT
	p.name
FROM
	people AS p
JOIN
	stars AS s
		ON p.id = s.person_id
JOIN
	movies AS m
		ON m.id = s.movie_id
WHERE
	m.year = 2004
GROUP BY
	s.person_id
ORDER BY
	p.birth;